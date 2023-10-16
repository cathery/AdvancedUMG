// Copyright (c) 2022 cathery

#include "UMG/AdvBottomSheetSlot.h"

#include "Components/Widget.h"

#include "UMG/AdvBottomSheet.h"

UAdvBottomSheetSlot::UAdvBottomSheetSlot()
	: bIsPersistent(false)
{
}

void UAdvBottomSheetSlot::SetIsPersistent(bool bInIsPersistent)
{
	bIsPersistent = bInIsPersistent;
}

void UAdvBottomSheetSlot::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	SetIsPersistent(bIsPersistent);
}

void UAdvBottomSheetSlot::BuildSlot(const TSharedRef<SAdvBottomSheet> Container)
{
	Container->AddSheetSlot(bIsPersistent)
	.Expose(Slot)
	.Padding(Padding)
	.HAlign(HorizontalAlignment)
	.VAlign(VerticalAlignment)
	[
		Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
	];
}

void UAdvBottomSheetSlot::RemoveSlot(const TSharedRef<SAdvBottomSheet> Container)
{
	// Remove the widget from the live slot if it exists.
	if (Content)
	{
		const TSharedPtr<SWidget> Widget = Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			Container->RemoveSheetSlot(Widget.ToSharedRef(), bIsPersistent);
		}
	}
}
