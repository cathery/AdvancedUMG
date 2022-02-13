// Copyright (c) 2022 cathery

#include "UMG/AdvPanelBoxHorizontal.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"

#include "Slate/SAdvPanelBox.h"
#include "UMG/AdvPanelBoxSlot.h"

void UAdvPanelBoxHorizontal::SetSpacing(float InSpacing)
{
	Spacing = InSpacing;
	if (MyBoxPanel.IsValid())
	{
		MyBoxPanel->SetSpacing(InSpacing);
	}
}

UAdvPanelBoxHorizontal::UAdvPanelBoxHorizontal()
{
	bIsVariable = false;
	Visibility  = ESlateVisibility::SelfHitTestInvisible;
}

UClass* UAdvPanelBoxHorizontal::GetSlotClass() const
{
	return UAdvPanelBoxSlot::StaticClass();
}

void UAdvPanelBoxHorizontal::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyBoxPanel.IsValid())
	{
		CastChecked<UAdvPanelBoxSlot>(InSlot)->BuildSlot(MyBoxPanel.ToSharedRef());
	}
}

void UAdvPanelBoxHorizontal::OnSlotRemoved(UPanelSlot* InSlot)
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

TSharedRef<SWidget> UAdvPanelBoxHorizontal::RebuildWidget()
{
	MyBoxPanel = SNew(SAdvPanelBox).Orientation(Orient_Horizontal);

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

void UAdvPanelBoxHorizontal::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyBoxPanel->SetSpacing(Spacing);
}

void UAdvPanelBoxHorizontal::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyBoxPanel.Reset();
}

#if WITH_EDITOR

const FText UAdvPanelBoxHorizontal::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "AdvWidgets", "Advanced Widgets");
}

#endif
