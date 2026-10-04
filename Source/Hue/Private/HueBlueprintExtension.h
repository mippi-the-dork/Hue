// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/BlueprintExtension.h"
#include "HueTypes.h"
#include "HueBlueprintExtension.generated.h"

UCLASS()
class UHueBlueprintExtension : public UBlueprintExtension
{
    GENERATED_BODY()

public:
    /** Per-node styles stored with the owning Blueprint asset. */
    UPROPERTY()
    TMap<FGuid, FHueNodeStyleOverride> InstanceStyles;

    /** Blueprint member-category styles stored with the Blueprint that defines those members. */
    UPROPERTY()
    TMap<FString, FHueNodeStyleOverride> CategoryStyles;
};
