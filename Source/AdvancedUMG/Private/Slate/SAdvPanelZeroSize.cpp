// Copyright (c) 2022 cathery

#include "Slate/SAdvPanelZeroSize.h"

void SAdvPanelZeroSize::Construct(const FArguments& InArgs)
{
	// Call the parent constructor with our slots
	SAdvPanel::FArguments ParentArgs;
	ParentArgs.Slots = InArgs.Slots;
	SAdvPanel::Construct(ParentArgs);
}

FVector2D SAdvPanelZeroSize::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D::ZeroVector;
}
