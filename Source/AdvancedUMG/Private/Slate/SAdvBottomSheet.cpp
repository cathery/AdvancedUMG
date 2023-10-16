// Copyright (c) 2022 cathery

#include "Slate/SAdvBottomSheet.h"

#include "Layout/LayoutUtils.h"
#include "Widgets/Layout/SScrollBox.h"

#include "Slate/SAdvColorRect.h"

SLATE_IMPLEMENT_WIDGET(SAdvBottomSheet)
void SAdvBottomSheet::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SAdvBottomSheet::SAdvBottomSheet()
	: DragThreshold(0.0f)
	, MoveThreshold(0.0f)
	, SheetInterpSpeed(0.0f)
	, ScrollInterpSpeed(0.0f)
	, SheetSpeedMultiplier(1.0f)
	, ScrollSpeedMultiplier(1.0f)
	, SheetScrollPanel(nullptr)
	, SheetPanel(nullptr)
	, SheetPanelSlot(nullptr)
	, OffscreenClickBlocker(nullptr)
	, bIsPressed(false)
	, bIsDragging(false)
	, TotalMouseDelta(0.0f)
	, CurrentSheetOffset(0.0f)
	, TargetSheetOffset(0.0f)
	, SheetDelta(0.0f)
	, ScrollDelta(0.0f)
	, CurrentSheetTargetAreaIndex(0)
{
	SetCanTick(true);
}

void SAdvBottomSheet::Construct(const FArguments& InArgs)
{
	// Call the parent constructor
	Super::Construct(Super::FArguments());

	DragThreshold = InArgs._DragThreshold;
	MoveThreshold = InArgs._MoveThreshold;

	SheetInterpSpeed = InArgs._SheetInterpSpeed;
	ScrollInterpSpeed = InArgs._ScrollInterpSpeed;

	SheetSpeedMultiplier = InArgs._SheetSpeedMultiplier;
	ScrollSpeedMultiplier = InArgs._ScrollSpeedMultiplier;

	SheetTargetAreas = InArgs._SheetTargetAreas;

	SheetTargetAreas.Sort();
	for (auto& TargetArea : SheetTargetAreas)
	{
		TargetArea = FMath::Clamp(TargetArea, 0.0f, 1.0f);
	}
	CurrentSheetTargetAreaIndex = 0;

	AddSlot()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SAssignNew(OffscreenClickBlocker, SAdvColorRect)
		.ColorAndOpacity(FLinearColor::Transparent)
		.Visibility(EVisibility::SelfHitTestInvisible)
	];

	AddSlot()
	.Expose(SheetPanelSlot)
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SAssignNew(SheetPanel, SAdvPanel)
		.Visibility(EVisibility::SelfHitTestInvisible)
		.Clipping(EWidgetClipping::ClipToBounds)
		+ Slot()
		[
			SAssignNew(SheetScrollPanel, SAdvBottomSheetScrollPanel, InArgs._Slots)
			.Image(InArgs._Background)
		]
	];
}

SAdvBottomSheet::FScopedWidgetSlotArguments SAdvBottomSheet::AddSheetSlot(bool bIsPersistent)
{
	if (bIsPersistent)
	{
		return SheetPanel->AddSlot();
	}
	return SheetScrollPanel->AddSlot();
}

int32 SAdvBottomSheet::RemoveSheetSlot(const TSharedRef<SWidget>& SlotWidget, bool bIsPersistent)
{
	if (bIsPersistent)
	{
		return SheetPanel->RemoveSlot(SlotWidget);
	}
	return SheetScrollPanel->RemoveSlot(SlotWidget);
}

void SAdvBottomSheet::InitHeight(const TOptional<const FGeometry> MyGeometry)
{
	const FGeometry& Geometry = MyGeometry.Get(GetCachedGeometry());

	TargetSheetOffset = FindSheetMinimumHeight(Geometry);
	CurrentSheetOffset = TargetSheetOffset;

	TargetScrollOffset = FindScrollPanelMinimumOffset(Geometry);
	CurrentScrollOffset = TargetScrollOffset;
}

void SAdvBottomSheet::SetBackground(const FSlateBrush& InBackground)
{
	SheetScrollPanel->SetImage(&InBackground);
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SAdvBottomSheet::SetDragThreshold(float InDragThreshold)
{
	DragThreshold = InDragThreshold;
}

void SAdvBottomSheet::SetMoveThreshold(float InMoveThreshold)
{
	MoveThreshold = InMoveThreshold;
}

void SAdvBottomSheet::SetSheetInterpSpeed(float InSheetInterpSpeed)
{
	SheetInterpSpeed = InSheetInterpSpeed;
}

void SAdvBottomSheet::SetScrollInterpSpeed(float InScrollInterpSpeed)
{
	ScrollInterpSpeed = InScrollInterpSpeed;
}

void SAdvBottomSheet::SetSheetTargetAreas(const TArray<float>& InSheetTargetAreas)
{
	SheetTargetAreas = InSheetTargetAreas;
	SheetTargetAreas.Sort();
	for (auto& TargetArea : SheetTargetAreas)
	{
		TargetArea = FMath::Clamp(TargetArea, 0.0f, 1.0f);
	}
	CurrentSheetTargetAreaIndex = 0;
	InitHeight();
}

int32 SAdvBottomSheet::FindTargetAreaToSnapTo(const FGeometry& MyGeometry, float Height, float DeltaHeight)
{
	if (SheetTargetAreas.Num() > 0)
	{
		const int32 Step = FMath::Sign(DeltaHeight);
		if (Step != 0)
		{
			// This floors/ceils the current height to the closest height
			// and then moves it by one in the direction of the delta height
			const float TotalHeight = Height + DeltaHeight;

			for (int32 Index = 0; Index < SheetTargetAreas.Num(); Index++)
			{
				const int32 ResultIndex = (DeltaHeight > 0) ? SheetTargetAreas.Num() - 1 - Index : Index; 
				const float AreaHeight = SheetTargetAreas[ResultIndex] * MyGeometry.GetLocalSize().Y;
				if ((TotalHeight * Step) >= (AreaHeight * Step))
				{
					return FMath::Clamp(ResultIndex + Step, 0, SheetTargetAreas.Num() - 1);
				}
			}
		}
		return CurrentSheetTargetAreaIndex;
	}
	return -1;
}

float SAdvBottomSheet::FindSheetMinimumHeight(const FGeometry& MyGeometry) const
{
	return (SheetTargetAreas.Num() > 0 ? SheetTargetAreas[0] : 0.0f) * MyGeometry.GetLocalSize().Y;
}

float SAdvBottomSheet::FindSheetMaximumHeight(const FGeometry& MyGeometry) const
{
	return (SheetTargetAreas.Num() > 0 ? SheetTargetAreas[SheetTargetAreas.Num() - 1] : 1.0f) * MyGeometry.GetLocalSize().Y;
}

float SAdvBottomSheet::FindScrollPanelMinimumOffset(const FGeometry& MyGeometry) const
{
	return 0.0f;
}

float SAdvBottomSheet::FindScrollPanelMaximumOffset(const FGeometry& MyGeometry) const
{
	const FGeometry& ScrollPanelGeometry = FindChildGeometry(MyGeometry, SheetScrollPanel.ToSharedRef());
	const float ContentSize = SheetScrollPanel->GetDesiredSize().Y;
	return FMath::Max(ContentSize - ScrollPanelGeometry.Size.Y, 0.0f);
}

float SAdvBottomSheet::FindTotalMinimumOffset(const FGeometry& MyGeometry) const
{
	return FindSheetMinimumHeight(MyGeometry);
}

float SAdvBottomSheet::FindTotalMaximumOffset(const FGeometry& MyGeometry) const
{
	return FindSheetMaximumHeight(MyGeometry) + FindScrollPanelMaximumOffset(MyGeometry);
}

void SAdvBottomSheet::SetSheetOffset(float InSheetOffset)
{
	TargetSheetOffset = InSheetOffset;
	CurrentSheetOffset = InSheetOffset;
}

void SAdvBottomSheet::SetScrollOffset(float InScrollOffset)
{
	TargetScrollOffset = InScrollOffset;
	CurrentScrollOffset = InScrollOffset;
}

void SAdvBottomSheet::SetSheetSpeedMultiplier(float InSheetSpeedMultiplier)
{
	SheetSpeedMultiplier = InSheetSpeedMultiplier;
}

void SAdvBottomSheet::SetScrollSpeedMultiplier(float InScrollSpeedMultiplier)
{
	ScrollSpeedMultiplier = InScrollSpeedMultiplier;
}

void SAdvBottomSheet::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	const FVector2D& CurrentSize = AllottedGeometry.GetLocalSize();

	// If the geometry of the widget changed, update the height values
	if (CurrentSize != GetCachedGeometry().GetLocalSize())
	{
		InitHeight(AllottedGeometry);
	}

	const float SheetOffset = FMath::FInterpTo(CurrentSheetOffset, TargetSheetOffset + SheetDelta, InDeltaTime, SheetInterpSpeed);
	CurrentSheetOffset = FMath::Clamp(SheetOffset, FindSheetMinimumHeight(AllottedGeometry), FindSheetMaximumHeight(AllottedGeometry));

	const float ScrollOffset = FMath::FInterpTo(CurrentScrollOffset, TargetScrollOffset + ScrollDelta, InDeltaTime, ScrollInterpSpeed);
	CurrentScrollOffset = FMath::Clamp(ScrollOffset, FindScrollPanelMinimumOffset(AllottedGeometry), FindScrollPanelMaximumOffset(AllottedGeometry));


	SheetPanelSlot->SetPadding(FMargin(0.0f,  CurrentSize.Y - CurrentSheetOffset, 0.0f, 0.0f));
	SheetScrollPanel->PhysicalOffset = CurrentScrollOffset;
}

FReply SAdvBottomSheet::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
		TotalMouseDelta = 0.0f;
		// ScrollDelta = 0.0f;
		// SheetDelta = 0.0f;

		const FGeometry& SheetGeometry = FindChildGeometry(MyGeometry, SheetScrollPanel.ToSharedRef());
		bIsPressed = SheetGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition());
	}
	return FReply::Unhandled();
}

FReply SAdvBottomSheet::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SAdvBottomSheet::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!bIsPressed)
		{
			const FGeometry& SheetGeometry = FindChildGeometry(MyGeometry, SheetScrollPanel.ToSharedRef());
			if (!SheetGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition()))
			{
				CurrentSheetTargetAreaIndex = 0;
				TargetSheetOffset = FindSheetMinimumHeight(MyGeometry);
			}
		}
		else if (bIsDragging)
		{
			CurrentSheetTargetAreaIndex = FindTargetAreaToSnapTo(MyGeometry, TargetSheetOffset, SheetDelta);
			TargetSheetOffset = SheetTargetAreas[CurrentSheetTargetAreaIndex] * MyGeometry.GetLocalSize().Y;

			TargetScrollOffset = FMath::Clamp(TargetScrollOffset + ScrollDelta, FindScrollPanelMinimumOffset(MyGeometry), FindScrollPanelMaximumOffset(MyGeometry));
		}

		OffscreenClickBlocker->SetVisibility(CurrentSheetTargetAreaIndex > 0 ? EVisibility::Visible : EVisibility::SelfHitTestInvisible);
		bIsDragging = false;
		bIsPressed = false;
		TotalMouseDelta = 0.0f;
		ScrollDelta = 0.0f;
		SheetDelta = 0.0f;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply SAdvBottomSheet::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	if (bIsPressed)
	{
		Reply = FReply::Handled();

		if (!HasMouseCapture())
		{
			Reply.CaptureMouse(AsShared());
		}

		const float ScrollByAmountLocal = -(MouseEvent.GetCursorDelta().Y / MyGeometry.GetAccumulatedLayoutTransform().GetScale());
		if (!bIsDragging)
		{
			TotalMouseDelta += ScrollByAmountLocal;
			if (FMath::Abs(TotalMouseDelta) >= DragThreshold)
			{
				bIsDragging = true;
				// Drag started
			}
		}

		if (bIsDragging)
		{
			if (ScrollByAmountLocal > 0)
			{
				// Scroll up
				// when scrolling up, we always want to move sheet first, content second

				const float NeededSheetDelta = FMath::Max(FindSheetMaximumHeight(MyGeometry) - TargetSheetOffset - SheetDelta, 0.0f);
				const float UsedSheetDelta = FMath::Min(ScrollByAmountLocal * SheetSpeedMultiplier, NeededSheetDelta);
				const float ExcessSheetDelta = FMath::Max(ScrollByAmountLocal - UsedSheetDelta, 0.0f);

				SheetDelta += UsedSheetDelta;

				const float NeededScrollDelta = FMath::Max(FindScrollPanelMaximumOffset(MyGeometry) - TargetScrollOffset - ScrollDelta, 0.0f);
				const float UsedScrollDelta = FMath::Min(ExcessSheetDelta * ScrollSpeedMultiplier, NeededScrollDelta);

				ScrollDelta += UsedScrollDelta;
			}
			else if (ScrollByAmountLocal < 0)
			{
				// Scroll down
				// when scrolling down, we always want to move content first, sheet second

				const float NeededScrollDelta = FMath::Min(FindScrollPanelMinimumOffset(MyGeometry) - TargetScrollOffset - ScrollDelta, 0.0f);
				const float UsedScrollDelta = FMath::Max(ScrollByAmountLocal * ScrollSpeedMultiplier, NeededScrollDelta);
				const float ExcessScrollDelta = FMath::Min(ScrollByAmountLocal - UsedScrollDelta, 0.0f);

				ScrollDelta += UsedScrollDelta;

				const float NeededSheetDelta = FMath::Min(FindSheetMinimumHeight(MyGeometry) - TargetSheetOffset - SheetDelta, 0.0f);
				const float UsedSheetDelta = FMath::Max(ExcessScrollDelta * SheetSpeedMultiplier, NeededSheetDelta);

				SheetDelta += UsedSheetDelta;
			}
		}
	}
	return Reply;
}

void SAdvBottomSheet::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::OnMouseCaptureLost(CaptureLostEvent);
	bIsPressed = false;
	bIsDragging = false;
	TotalMouseDelta = 0.0f;
}

bool SAdvBottomSheet::IsInteractable() const
{
	return IsEnabled();
}

FVector2D SAdvBottomSheet::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D::ZeroVector;
}

SAdvBottomSheetScrollPanel::SAdvBottomSheetScrollPanel()
	: PhysicalOffset(0.0f)
{
}

void SAdvBottomSheetScrollPanel::Construct(const FArguments& InArgs, const TArray<FSlot::FSlotArguments>& InSlots)
{
	// FArguments ArgsCopy(InArgs);
	// ArgsCopy._Slots = InSlots;
	// SAdvImage::Construct(ArgsCopy);

	PhysicalOffset = 0.0f;
}

void SAdvBottomSheetScrollPanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const FVector2D& GeometryOffset = FVector2D(0.0f, -PhysicalOffset);
	const FGeometry& OffsetGeometry = AllottedGeometry.MakeChild(AllottedGeometry.GetLocalSize() - GeometryOffset, FSlateLayoutTransform(GeometryOffset));
	SAdvImage::OnArrangeChildren(OffsetGeometry, ArrangedChildren);
}
