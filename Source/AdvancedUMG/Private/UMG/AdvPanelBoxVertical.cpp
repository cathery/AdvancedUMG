// Copyright (c) 2022 cathery

#include "UMG/AdvPanelBoxVertical.h"

#include "Slate/SAdvPanelBox.h"
#include "UMG/AdvPanelBoxSlot.h"

void UAdvPanelBoxVertical::SetSpacing(float InSpacing)
{
	Spacing = InSpacing;
	if (MyBoxPanel.IsValid())
	{
		MyBoxPanel->SetSpacing(InSpacing);
	}
}

UAdvPanelBoxVertical::UAdvPanelBoxVertical()
{
	bIsVariable = false;
	Visibility  = ESlateVisibility::SelfHitTestInvisible;
}

UClass* UAdvPanelBoxVertical::GetSlotClass() const
{
	return UAdvPanelBoxSlot::StaticClass();
}

void UAdvPanelBoxVertical::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyBoxPanel.IsValid())
	{
		CastChecked<UAdvPanelBoxSlot>(InSlot)->BuildSlot(MyBoxPanel.ToSharedRef());
	}
}

void UAdvPanelBoxVertical::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyBoxPanel.IsValid() && InSlot->Content)
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyBoxPanel->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UAdvPanelBoxVertical::RebuildWidget()
{
	MyBoxPanel = SNew(SAdvPanelBox).Orientation(Orient_Vertical);

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UAdvPanelBoxSlot* TypedSlot = Cast<UAdvPanelBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyBoxPanel.ToSharedRef());
		}
	}

	return MyBoxPanel.ToSharedRef();
}

void UAdvPanelBoxVertical::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyBoxPanel->SetSpacing(Spacing);
}

void UAdvPanelBoxVertical::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyBoxPanel.Reset();
}

#if WITH_EDITOR

const FText UAdvPanelBoxVertical::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "AdvWidgets", "Advanced Widgets");
}

#endif
