// Copyright (c) 2022 cathery

#include "UMG/AdvBottomSheet.h"

#include "Slate/SAdvBottomSheet.h"
#include "UMG/AdvBottomSheetSlot.h"

UAdvBottomSheet::UAdvBottomSheet()
	: DragThreshold(30.0f)
	, MoveThreshold(60.0f)
	, SheetInterpSpeed(20.0f)
	, ScrollInterpSpeed(20.0f)
	, SheetSpeedMultiplier(1.0f)
	, ScrollSpeedMultiplier(1.0f)
	, SheetTargetAreas({0.1f, 0.25f, 0.75f})
{
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);

#if WITH_EDITORONLY_DATA
	SheetOffset = 0.0f;
	ScrollOffset = 0.0f;
#endif
}

void UAdvBottomSheet::SetBackground(const FSlateBrush& InBackground)
{
	if (Background != InBackground)
	{
		Background = InBackground;

		if (MySheet)
		{
			MySheet->SetBackground(InBackground);
		}
	}
}

void UAdvBottomSheet::SetDragThreshold(float InDragThreshold)
{
	DragThreshold = InDragThreshold;
	if (MySheet)
	{
		MySheet->SetDragThreshold(InDragThreshold);
	}
}

void UAdvBottomSheet::SetMoveThreshold(float InMoveThreshold)
{
	MoveThreshold = InMoveThreshold;
	if (MySheet)
	{
		MySheet->SetMoveThreshold(InMoveThreshold);
	}
}

void UAdvBottomSheet::SetSheetInterpSpeed(float InSheetInterpSpeed)
{
	SheetInterpSpeed = InSheetInterpSpeed;
	if (MySheet)
	{
		MySheet->SetSheetInterpSpeed(InSheetInterpSpeed);
	}
}

void UAdvBottomSheet::SetScrollInterpSpeed(float InScrollInterpSpeed)
{
	ScrollInterpSpeed = InScrollInterpSpeed;
	if (MySheet)
	{
		MySheet->SetScrollInterpSpeed(InScrollInterpSpeed);
	}
}

void UAdvBottomSheet::SetSheetSpeedMultiplier(float InSheetSpeedMultiplier)
{
	SheetSpeedMultiplier = InSheetSpeedMultiplier;
	if (MySheet)
	{
		MySheet->SetSheetSpeedMultiplier(InSheetSpeedMultiplier);
	}
}

void UAdvBottomSheet::SetScrollSpeedMultiplier(float InScrollSpeedMultiplier)
{
	ScrollSpeedMultiplier = InScrollSpeedMultiplier;
	if (MySheet)
	{
		MySheet->SetScrollSpeedMultiplier(InScrollSpeedMultiplier);
	}
}

void UAdvBottomSheet::SetSheetTargetAreas(const TArray<float>& InSheetTargetAreas)
{
	SheetTargetAreas = InSheetTargetAreas;
	if (MySheet)
	{
		MySheet->SetSheetTargetAreas(InSheetTargetAreas);
	}
}

void UAdvBottomSheet::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MySheet)
	{
		MySheet->SetBackground(Background);
		MySheet->SetDragThreshold(DragThreshold);
		MySheet->SetMoveThreshold(MoveThreshold);
		MySheet->SetSheetInterpSpeed(SheetInterpSpeed);
		MySheet->SetScrollInterpSpeed(ScrollInterpSpeed);
		MySheet->SetSheetTargetAreas(SheetTargetAreas);
		MySheet->SetSheetSpeedMultiplier(SheetSpeedMultiplier);
		MySheet->SetScrollSpeedMultiplier(ScrollSpeedMultiplier);

#if WITH_EDITORONLY_DATA
		MySheet->SetSheetOffset(SheetOffset);
		MySheet->SetScrollOffset(ScrollOffset);
#endif
	}
}

void UAdvBottomSheet::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySheet.Reset();
}

UClass* UAdvBottomSheet::GetSlotClass() const
{
	return UAdvBottomSheetSlot::StaticClass();
}

void UAdvBottomSheet::OnSlotAdded(UPanelSlot* InSlot)
{
	if (GetSlateAdvBottomSheet())
	{
		CastChecked<UAdvBottomSheetSlot>(InSlot)->BuildSlot(GetSlateAdvBottomSheet().ToSharedRef());
	}
}

void UAdvBottomSheet::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (GetSlateAdvBottomSheet())
	{
		CastChecked<UAdvBottomSheetSlot>(InSlot)->RemoveSlot(GetSlateAdvBottomSheet().ToSharedRef());
	}
}

TSharedRef<SWidget> UAdvBottomSheet::RebuildWidget()
{
	MakeSlateWidget();

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UAdvBottomSheetSlot* TypedSlot = Cast<UAdvBottomSheetSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(GetSlateAdvBottomSheet().ToSharedRef());
		}
	}

	return GetSlateAdvBottomSheet().ToSharedRef();
}

TSharedPtr<SAdvPanel> UAdvBottomSheet::GetSlateAdvPanel() const
{
	return GetSlateAdvBottomSheet();
}

TSharedPtr<SAdvBottomSheet> UAdvBottomSheet::GetSlateAdvBottomSheet() const
{
	return MySheet;
}

void UAdvBottomSheet::MakeSlateWidget()
{
	MySheet = SNew(SAdvBottomSheet);
}
