// Copyright (c) 2022 cathery

#include "UMG/AdvPanelPadding.h"

#include "Slate/SAdvPanelPadding.h"

void UAdvPanelPadding::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (MyPanel.IsValid())
	{
		StaticCastSharedPtr<SAdvPanelPadding>(MyPanel)->SetPadding(InPadding);
	}
}

void UAdvPanelPadding::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	StaticCastSharedPtr<SAdvPanelPadding>(MyPanel)->SetPadding(Padding);
}

void UAdvPanelPadding::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPanel.Reset();
}

TSharedPtr<SAdvPanel> UAdvPanelPadding::GetSlateAdvPanel() const
{
	return MyPanel;
}

void UAdvPanelPadding::MakeSlateWidget()
{
	MyPanel = SNew(SAdvPanelPadding);
}
