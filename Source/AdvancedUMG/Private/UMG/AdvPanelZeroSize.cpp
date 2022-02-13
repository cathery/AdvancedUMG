// Copyright (c) 2022 cathery

#include "UMG/AdvPanelZeroSize.h"

#include "Slate/SAdvPanelZeroSize.h"

void UAdvPanelZeroSize::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPanel.Reset();
}

TSharedPtr<SAdvPanel> UAdvPanelZeroSize::GetSlateAdvPanel() const
{
	return MyPanel;
}

void UAdvPanelZeroSize::MakeSlateWidget()
{
	MyPanel = SNew(SAdvPanelZeroSize);
}
