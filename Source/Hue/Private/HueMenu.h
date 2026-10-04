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
    static void BuildHueMenu(UToolMenu* Menu, TWeakObjectPtr<UEdGraphNode> WeakNode);
    static void BuildScopeMenu(UToolMenu* Menu, TWeakObjectPtr<UEdGraphNode> WeakNode, EHueStyleScope Scope);
    static void AddChannelEntries(FToolMenuSection& Section, TWeakObjectPtr<UEdGraphNode> WeakNode, EHueStyleScope Scope, EHueStyleChannel Channel, const FText& Label, FName NamePrefix);
    static void OpenHueColorPicker(TWeakObjectPtr<UEdGraphNode> WeakNode, EHueStyleScope Scope, EHueStyleChannel Channel);
};
