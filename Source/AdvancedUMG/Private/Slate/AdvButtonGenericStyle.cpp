// Copyright (c) 2022 cathery

#include "Slate/AdvButtonGenericStyle.h"

#include "SlateCore/Public/Brushes/SlateBoxBrush.h"

const FName FAdvButtonGenericStyle::TypeName(TEXT("FAdvGenericButtonStyle"));

FAdvButtonGenericStyle::FAdvButtonGenericStyle()
	: Disabled(FSlateBrush(FSlateNoResource()))
{
}

void FAdvButtonGenericStyle::GetResources( TArray< const FSlateBrush* >& OutBrushes ) const
{
	OutBrushes.Add( &Normal );
	OutBrushes.Add( &Hovered );
	OutBrushes.Add( &Pressed );
	OutBrushes.Add( &Disabled );
}

const FAdvButtonGenericStyle& FAdvButtonGenericStyle::GetDefault()
{
	static FAdvButtonGenericStyle Default;
	return Default;
}
