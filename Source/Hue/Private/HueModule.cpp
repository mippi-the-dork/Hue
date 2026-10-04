// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueModule.h"

#include "BlueprintEditor.h"
#include "BlueprintEditorModule.h"
#include "BlueprintEditorTabs.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "Framework/Docking/LayoutExtender.h"
#include "Framework/Docking/TabManager.h"
#include "HueMenu.h"
#include "HueNodeFactory.h"
#include "HuePinFactory.h"
#include "HueTabSummoner.h"
#include "K2Node.h"
#include "Modules/ModuleManager.h"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "UObject/UObjectHash.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

#define LOCTEXT_NAMESPACE "HueModule"

void FHueModule::StartupModule()
{
    NodeFactory = MakeShared<FHueNodeFactory>();
    FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);

    // The pin factory matters most for native-specialized node widgets. Those
    // widgets still ask FNodeFactory for their pins, so Hue can preserve the
    // native node presentation while tinting execution pins safely.
    PinFactory = MakeShared<FHuePinFactory>();
    FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);

    ToolMenusStartupHandle = UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(
            this,
            &FHueModule::RegisterMenus));

    FBlueprintEditorModule& BlueprintEditorModule =
        FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");

    RegisterBlueprintTabsHandle =
        BlueprintEditorModule.OnRegisterTabsForEditor().AddRaw(
            this,
            &FHueModule::RegisterBlueprintTabs);

    ExtendBlueprintLayoutHandle =
        BlueprintEditorModule.OnRegisterLayoutExtensions().AddRaw(
            this,
            &FHueModule::ExtendBlueprintLayout);

    // Blueprint graph context menus are not all guaranteed to exist when the
    // ToolMenus startup callback fires. Refresh after a Blueprint editor has
    // finished initialization, when its graph menu hierarchy is available.
    BlueprintEditorOpenedHandle =
        BlueprintEditorModule.OnBlueprintEditorOpened().AddLambda(
            [this](EBlueprintType)
            {
                RefreshNodeContextMenus();
            });

    ModulesChangedHandle =
        FModuleManager::Get().OnModulesChanged().AddRaw(
            this,
            &FHueModule::HandleModulesChanged);
}

void FHueModule::ShutdownModule()
{
    if (PinFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
        PinFactory.Reset();
    }

    if (NodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
        NodeFactory.Reset();
    }

    if (ModulesChangedHandle.IsValid())
    {
        FModuleManager::Get().OnModulesChanged().Remove(ModulesChangedHandle);
        ModulesChangedHandle.Reset();
    }

    if (ToolMenusStartupHandle.IsValid())
    {
        UToolMenus::UnRegisterStartupCallback(ToolMenusStartupHandle);
        ToolMenusStartupHandle.Reset();
    }

    if (UToolMenus* ToolMenus = UToolMenus::TryGet())
    {
        ToolMenus->UnregisterOwner(this);
    }

    RegisteredHueContextMenus.Reset();
    bToolMenusReady = false;

    if (FModuleManager::Get().IsModuleLoaded("Kismet"))
    {
        FBlueprintEditorModule& BlueprintEditorModule =
            FModuleManager::GetModuleChecked<FBlueprintEditorModule>("Kismet");

        if (BlueprintEditorOpenedHandle.IsValid())
        {
            BlueprintEditorModule.OnBlueprintEditorOpened().Remove(
                BlueprintEditorOpenedHandle);
            BlueprintEditorOpenedHandle.Reset();
        }

        if (RegisterBlueprintTabsHandle.IsValid())
        {
            BlueprintEditorModule.OnRegisterTabsForEditor().Remove(
                RegisterBlueprintTabsHandle);
            RegisterBlueprintTabsHandle.Reset();
        }

        if (ExtendBlueprintLayoutHandle.IsValid())
        {
            BlueprintEditorModule.OnRegisterLayoutExtensions().Remove(
                ExtendBlueprintLayoutHandle);
            ExtendBlueprintLayoutHandle.Reset();
        }
    }
}

void FHueModule::RegisterMenus()
{
    bToolMenusReady = true;
    RegisterLoadedNodeContextMenus();
}

void FHueModule::RefreshNodeContextMenus()
{
    if (!bToolMenusReady)
    {
        return;
    }

    // Menu definitions can be rebuilt during Blueprint editor startup. Remove
    // only Hue-owned ToolMenus entries, forget the previous registration set,
    // then restore Hue's extensions for every expected Blueprint node menu name.
    if (UToolMenus* ToolMenus = UToolMenus::TryGet())
    {
        ToolMenus->UnregisterOwner(this);
    }

    RegisteredHueContextMenus.Reset();
    RegisterLoadedNodeContextMenus();
}

void FHueModule::RegisterContextMenuName(FName ContextMenuName)
{
    if (!bToolMenusReady
        || ContextMenuName.IsNone()
        || RegisteredHueContextMenus.Contains(ContextMenuName))
    {
        return;
    }

    UToolMenus* ToolMenus = UToolMenus::TryGet();
    if (!ToolMenus)
    {
        return;
    }

    // ExtendMenu is intentionally valid even before Unreal registers the menu.
    // Blueprint graph node context menus are often registered lazily, so
    // requiring IsMenuRegistered() here causes Hue to miss those menus entirely.
    // Register the extension now and let ToolMenus merge it when the native menu
    // becomes available.
    FToolMenuOwnerScoped OwnerScoped(this);
    if (UToolMenu* Menu = ToolMenus->ExtendMenu(ContextMenuName))
    {
        FToolMenuSection& Section = Menu->AddSection(
            TEXT("Hue"),
            LOCTEXT("HueSectionHeader", "Hue"));

        Section.AddDynamicEntry(
            TEXT("HueDynamicEntry"),
            FNewToolMenuSectionDelegate::CreateStatic(
                &FHueMenu::BuildNodeContextEntry));

        RegisteredHueContextMenus.Add(ContextMenuName);
    }
}

void FHueModule::RegisterLoadedNodeContextMenus()
{
    if (!bToolMenusReady)
    {
        return;
    }

    // Extend the K2 parent menu first. Class-specific graph-node menus inherit
    // from this hierarchy, making Hue resilient to node classes that are loaded
    // after startup or supplied by other editor modules.
    if (const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>())
    {
        RegisterContextMenuName(K2Schema->GetParentContextMenuName());
    }

    TArray<UClass*> NodeClasses;
    GetDerivedClasses(UK2Node::StaticClass(), NodeClasses, true);
    NodeClasses.AddUnique(UK2Node::StaticClass());

    for (UClass* NodeClass : NodeClasses)
    {
        if (!NodeClass)
        {
            continue;
        }

        RegisterContextMenuName(
            UEdGraphSchema::GetContextMenuName(NodeClass));
    }
}

void FHueModule::HandleModulesChanged(
    FName ModuleName,
    EModuleChangeReason ChangeReason)
{
    if (ChangeReason == EModuleChangeReason::ModuleLoaded)
    {
        // Some Blueprint node classes live in optional editor modules. Rescan
        // after those modules register their node classes and context menus.
        RegisterLoadedNodeContextMenus();
    }
}

void FHueModule::RegisterBlueprintTabs(
    FWorkflowAllowedTabSet& TabFactories,
    FName ModeName,
    TSharedPtr<FBlueprintEditor> BlueprintEditor)
{
    if (!BlueprintEditor.IsValid())
    {
        return;
    }

    TabFactories.RegisterFactory(
        MakeShared<FHueTabSummoner>(BlueprintEditor));
}

void FHueModule::ExtendBlueprintLayout(FLayoutExtender& LayoutExtender)
{
    LayoutExtender.ExtendLayout(
        FTabId(FBlueprintEditorTabs::DetailsID),
        ELayoutExtensionPosition::After,
        FTabManager::FTab(
            FHueTabSummoner::TabID,
            ETabState::ClosedTab));
}

IMPLEMENT_MODULE(FHueModule, Hue)

#undef LOCTEXT_NAMESPACE
