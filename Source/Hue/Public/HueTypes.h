// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HueTypes.generated.h"

USTRUCT()
struct FHueNodeStyleOverride
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Hue")
    bool bOverrideHeaderColor = false;

    UPROPERTY(EditAnywhere, Category = "Hue", meta = (EditCondition = "bOverrideHeaderColor"))
    FLinearColor HeaderColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, Category = "Hue")
    bool bOverrideHeaderTextColor = false;

    UPROPERTY(EditAnywhere, Category = "Hue", meta = (EditCondition = "bOverrideHeaderTextColor"))
    FLinearColor HeaderTextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, Category = "Hue")
    bool bOverrideBodyColor = false;

    UPROPERTY(EditAnywhere, Category = "Hue", meta = (EditCondition = "bOverrideBodyColor"))
    FLinearColor BodyColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, Category = "Hue")
    bool bOverrideBodyTextColor = false;

    UPROPERTY(EditAnywhere, Category = "Hue", meta = (EditCondition = "bOverrideBodyTextColor"))
    FLinearColor BodyTextColor = FLinearColor::White;

    bool IsEmpty() const
    {
        return !bOverrideHeaderColor
            && !bOverrideHeaderTextColor
            && !bOverrideBodyColor
            && !bOverrideBodyTextColor;
    }
};

enum class EHueStyleChannel : uint8
{
    HeaderColor,
    HeaderTextColor,
    BodyColor,
    BodyTextColor
};

enum class EHueStyleScope : uint8
{
    Instance,
    Category,
    GlobalFunction
};
