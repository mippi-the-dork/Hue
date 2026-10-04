// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueSettings.h"

#include "Framework/Application/SlateApplication.h"
#include "UObject/UnrealType.h"

#if WITH_EDITOR
void UHueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().InvalidateAllWidgets(false);
    }
}


void UHueSettings::PostEditUndo()
{
    Super::PostEditUndo();
    SaveConfig();

    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().InvalidateAllWidgets(false);
    }
}
#endif
