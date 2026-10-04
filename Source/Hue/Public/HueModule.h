// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "Containers/Set.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"

class FBlueprintEditor;
class FLayoutExtender;
class FWorkflowAllowedTabSet;
struct FGraphPanelNodeFactory;
struct FGraphPanelPinFactory;

class FHueModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterMenus();
    void RefreshNodeContextMenus();
    void RegisterLoadedNodeContextMenus();
    void RegisterContextMenuName(FName ContextMenuName);
    void HandleModulesChanged(FName ModuleName, EModuleChangeReason ChangeReason);

    void RegisterBlueprintTabs(
        FWorkflowAllowedTabSet& TabFactories,
        FName ModeName,
        TSharedPtr<FBlueprintEditor> BlueprintEditor);

    void ExtendBlueprintLayout(FLayoutExtender& LayoutExtender);

    TSharedPtr<FGraphPanelNodeFactory> NodeFactory;
    TSharedPtr<FGraphPanelPinFactory> PinFactory;
    FDelegateHandle ToolMenusStartupHandle;
    FDelegateHandle RegisterBlueprintTabsHandle;
    FDelegateHandle ExtendBlueprintLayoutHandle;
    FDelegateHandle BlueprintEditorOpenedHandle;
    FDelegateHandle ModulesChangedHandle;

    TSet<FName> RegisteredHueContextMenus;
    bool bToolMenusReady = false;
};
