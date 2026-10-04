// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Timeline.h"

#include "Components/TimelineComponent.h"
#include "K2Node_Timeline.h"
#include "Kismet2/KismetDebugUtilities.h"
#include "KismetNodes/KismetNodeInfoContext.h"
#include "SNodePanel.h"
#include "UObject/UnrealType.h"

void SHueGraphNodeK2Timeline::Construct(
    const FArguments& InArgs,
    UK2Node_Timeline* InNode)
{
    SHueGraphNodeCallFunction::Construct(
        SHueGraphNodeCallFunction::FArguments(),
        InNode);
}

void SHueGraphNodeK2Timeline::GetNodeInfoPopups(
    FNodeInfoContext* Context,
    TArray<FGraphInformationPopupInfo>& Popups) const
{
    // Preserve all normal K2 latent/watch information first.
    SHueGraphNodeCallFunction::GetNodeInfoPopups(Context, Popups);

    FKismetNodeInfoContext* K2Context =
        static_cast<FKismetNodeInfoContext*>(Context);
    if (!K2Context
        || !K2Context->SourceBlueprint
        || !K2Context->ActiveObjectBeingDebugged)
    {
        return;
    }

    FProperty* NodeProperty = FKismetDebugUtilities::FindClassPropertyForNode(
        K2Context->SourceBlueprint,
        GraphNode);

    FObjectPropertyBase* TimelineProperty =
        CastField<FObjectPropertyBase>(NodeProperty);
    if (!TimelineProperty)
    {
        return;
    }

    UObject* TimelineObject = TimelineProperty->GetObjectPropertyValue_InContainer(
        K2Context->ActiveObjectBeingDebugged);
    UTimelineComponent* Timeline = Cast<UTimelineComponent>(TimelineObject);
    if (!Timeline)
    {
        return;
    }

    const UK2Node_Timeline* TimelineNode = Cast<UK2Node_Timeline>(GraphNode);
    const FString TimelineName = TimelineNode
        ? TimelineNode->TimelineName.ToString()
        : FString(TEXT("Timeline"));

    const TCHAR* PlayState = Timeline->IsPlaying()
        ? TEXT("Playing")
        : TEXT("Stopped");
    const TCHAR* Direction = Timeline->IsReversing()
        ? TEXT("Reverse")
        : TEXT("Forward");
    const TCHAR* LoopState = Timeline->IsLooping()
        ? TEXT("Looping")
        : TEXT("Not Looping");

    const FString TimelineText = FString::Printf(
        TEXT("%s\n%s, %s\n%.2fs / %.2fs, %s"),
        *TimelineName,
        PlayState,
        Direction,
        Timeline->GetPlaybackPosition(),
        Timeline->GetTimelineLength(),
        LoopState);

    new (Popups) FGraphInformationPopupInfo(
        nullptr,
        TimelineBubbleColor,
        TimelineText);
}
