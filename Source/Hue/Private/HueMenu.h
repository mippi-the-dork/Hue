// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "HueTypes.h"

struct FToolMenuSection;
class UToolMenu;
class UEdGraphNode;

class FHueMenu
{
public:
    static void BuildNodeContextEntry(FToolMenuSection& Section);

private:
    static TArray<TWeakObjectPtr<UEdGraphNode>> ResolveContextNodes(
        UEdGraphNode* ContextNode);

    static void BuildHueMenu(
        UToolMenu* Menu,
        TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes);

    static void BuildScopeMenu(
        UToolMenu* Menu,
        TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
        EHueStyleScope Scope);

    static void AddChannelEntries(
        FToolMenuSection& Section,
        TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
        EHueStyleScope Scope,
        EHueStyleChannel Channel,
        const FText& Label,
        FName NamePrefix);

    static void OpenHueColorPicker(
        TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
        EHueStyleScope Scope,
        EHueStyleChannel Channel);
};
