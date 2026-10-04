// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HueTypes.h"

class UBlueprint;
class UEdGraph;
class UHueBlueprintExtension;
class UEdGraphNode;
class UK2Node;

struct FHueBlueprintCategoryInfo
{
    UBlueprint* DefiningBlueprint = nullptr;
    UEdGraph* MemberGraph = nullptr;
    FString Category;

    bool IsAvailable() const
    {
        return DefiningBlueprint != nullptr
            && !Category.IsEmpty();
    }
};

class FHueStyleResolver
{
public:
    /**
     * True only after Hue has a confirmed visual path for this live node.
     * UI surfaces use this so Hue never offers controls that cannot render.
     */
    static bool IsSupportedNode(const UEdGraphNode* Node);

    /**
     * Broad capability check used while constructing graph widgets. This says
     * Hue knows how to attempt this K2 presentation; IsSupportedNode() becomes
     * true only after that presentation path has actually been established.
     */
    static bool CanStyleNode(const UEdGraphNode* Node);

    /** Record that Hue successfully owns or decorated this node presentation. */
    static void MarkVisualSupport(UEdGraphNode* Node);

    /** Remove a provisional support claim when native decoration cannot attach. */
    static void UnmarkVisualSupport(UEdGraphNode* Node);

    /**
     * Node-owned visual widgets bypass registered node factories. These known
     * families use a deferred native-widget decoration bridge instead.
     */
    static bool UsesSelfVisualBridge(const UEdGraphNode* Node);

    static bool ResolveColor(const UEdGraphNode* Node, EHueStyleChannel Channel, FLinearColor& OutColor);
    static FLinearColor GetPickerInitialColor(const UEdGraphNode* Node, EHueStyleScope Scope, EHueStyleChannel Channel);

    /**
     * Returns a project-wide identity key for the selected Hue target.
     * Existing function keys intentionally remain raw UFunction paths for
     * compatibility with Global Function styles created by earlier builds.
     */
    static FString GetGlobalKey(const UEdGraphNode* Node);
    static FText GetGlobalScopeLabel(const UEdGraphNode* Node);
    static FText GetGlobalScopeTooltip(const UEdGraphNode* Node);

    /**
     * Resolves Hue's user-authored My Blueprint member category.
     * Supported for Blueprint-defined functions, macros, events, and member
     * variables when Unreal exposes a defining Blueprint member.
     */
    static bool GetBlueprintCategoryInfo(const UEdGraphNode* Node, FHueBlueprintCategoryInfo& OutInfo);
    static UBlueprint* GetDefiningBlueprint(const UEdGraphNode* Node);

    static UBlueprint* GetOwningBlueprint(const UEdGraphNode* Node);
    static UBlueprint* GetScopeBlueprint(const UEdGraphNode* Node, EHueStyleScope Scope);
    static UHueBlueprintExtension* FindBlueprintExtension(const UBlueprint* Blueprint);
    static UHueBlueprintExtension* GetOrCreateBlueprintExtension(UBlueprint* Blueprint);

    static void SetColorOverride(UEdGraphNode* Node, EHueStyleScope Scope, EHueStyleChannel Channel, const FLinearColor& Color);
    static void ClearColorOverride(UEdGraphNode* Node, EHueStyleScope Scope, EHueStyleChannel Channel);
    static void ClearAllOverrides(UEdGraphNode* Node, EHueStyleScope Scope);

    static bool HasColorOverride(const UEdGraphNode* Node, EHueStyleScope Scope, EHueStyleChannel Channel);
    static bool HasAnyOverride(const UEdGraphNode* Node, EHueStyleScope Scope);

private:
    static bool TryGetColor(const FHueNodeStyleOverride& Style, EHueStyleChannel Channel, FLinearColor& OutColor);
    static void SetColor(FHueNodeStyleOverride& Style, EHueStyleChannel Channel, const FLinearColor& Color);
    static void ClearColor(FHueNodeStyleOverride& Style, EHueStyleChannel Channel);
    static bool TryGetScopeStyle(const UEdGraphNode* Node, EHueStyleScope Scope, const FHueNodeStyleOverride*& OutStyle);
    static FLinearColor GetNativeColor(const UEdGraphNode* Node, EHueStyleChannel Channel);
};
