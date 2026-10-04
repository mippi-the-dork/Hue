// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "HueTypes.h"
#include "HueSettings.generated.h"

struct FPropertyChangedEvent;

UCLASS(Config = Editor, DefaultConfig, meta = (DisplayName = "Hue"))
class HUE_API UHueSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
    virtual FName GetSectionName() const override { return TEXT("Hue"); }

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostEditUndo() override;
#endif

    /** Project-shared styles. The property name is retained for config compatibility. */
    UPROPERTY(EditAnywhere, Config, Category = "Global Overrides", meta = (DisplayName = "Global Styles"))
    TMap<FString, FHueNodeStyleOverride> GlobalFunctionStyles;
};
