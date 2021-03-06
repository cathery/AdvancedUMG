// Copyright (c) 2022 cathery

#include "UMG/AdvPanelSlot.h"

#include "Components/Widget.h"

UAdvPanelSlot::UAdvPanelSlot()
	: Padding(0.0f)
	, HorizontalAlignment(HAlign_Fill)
	, VerticalAlignment(VAlign_Fill)
	, Slot(nullptr)
{
}

void UAdvPanelSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (Slot)
	{
		Slot->Padding(InPadding);
	}
}

void UAdvPanelSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (Slot)
	{
		Slot->HAlign(InHorizontalAlignment);
	}
}

void UAdvPanelSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (Slot)
	{
		Slot->VAlign(InVerticalAlignment);
	}
}

void UAdvPanelSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}

void UAdvPanelSlot::BuildSlot(TSharedRef<SAdvPanel> Container)
{
	Slot = &Container->AddSlot()
					 .Padding(Padding)
					 .HAlign(HorizontalAlignment)
					 .VAlign(VerticalAlignment)
	[
		Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
	];
}

void UAdvPanelSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	Slot = nullptr;
}
