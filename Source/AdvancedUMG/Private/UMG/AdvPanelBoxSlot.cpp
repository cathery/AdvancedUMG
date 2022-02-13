// Copyright (c) 2022 cathery

#include "UMG/AdvPanelBoxSlot.h"

#include "Components/Widget.h"

#include "Slate/SAdvPanelBox.h"

UAdvPanelBoxSlot::UAdvPanelBoxSlot()
	: Padding(0.0f)
	, Size(ESlateSizeRule::Automatic)
	, HorizontalAlignment(HAlign_Fill)
	, VerticalAlignment(VAlign_Fill)
	, Slot(nullptr)
{
}

void UAdvPanelBoxSlot::SetSize(FSlateChildSize InSize)
{
	Size = InSize;
	if (Slot)
	{
		Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(InSize);
	}
}

void UAdvPanelBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (Slot)
	{
		Slot->Padding(InPadding);
	}
}

void UAdvPanelBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (Slot)
	{
		Slot->HAlign(InHorizontalAlignment);
	}
}

void UAdvPanelBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (Slot)
	{
		Slot->VAlign(InVerticalAlignment);
	}
}

void UAdvPanelBoxSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetSize(Size);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}

void UAdvPanelBoxSlot::BuildSlot(TSharedRef<SAdvPanelBox> Container)
{
	Slot = &Container->AddSlot()
					 .Padding(Padding)
					 .HAlign(HorizontalAlignment)
					 .VAlign(VerticalAlignment)
	[
		Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
	];

	Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(Size);
}

void UAdvPanelBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	Slot = nullptr;
}
