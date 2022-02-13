// Copyright (c) 2022 cathery

#include "UMG/AdvPanel.h"

#include "Slate/SAdvPanel.h"

void UAdvPanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPanel.Reset();
}

TSharedPtr<SAdvPanel> UAdvPanel::GetSlateAdvPanel() const
{
	return MyPanel;
}

void UAdvPanel::MakeSlateWidget()
{
	MyPanel = SNew(SAdvPanel);
}
