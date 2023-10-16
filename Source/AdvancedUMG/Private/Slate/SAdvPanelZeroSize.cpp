// Copyright (c) 2022 cathery

#include "Slate/SAdvPanelZeroSize.h"

SLATE_IMPLEMENT_WIDGET(SAdvPanelZeroSize)
void SAdvPanelZeroSize::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}


void SAdvPanelZeroSize::Construct(const FArguments& InArgs)
{
	// Call the parent constructor with our slots
	Super::FArguments ParentArgs;
	ParentArgs._Slots = MoveTemp(const_cast<FArguments&>(InArgs)._Slots);
	Super::Construct(ParentArgs);
}

FVector2D SAdvPanelZeroSize::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D::ZeroVector;
}
