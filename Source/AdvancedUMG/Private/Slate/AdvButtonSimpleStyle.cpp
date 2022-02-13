// Copyright (c) 2022 cathery

#include "Slate/AdvButtonSimpleStyle.h"

#include "SlateCore/Public/Brushes/SlateBoxBrush.h"

const FName FAdvButtonSimpleStyle::TypeName( TEXT("FAdvSimpleButtonStyle") );

FAdvButtonSimpleStyle::FAdvButtonSimpleStyle()
	: DownNudge(0.0f)
	, bShowDisabledEffect(true)
{
}

void FAdvButtonSimpleStyle::GetResources( TArray< const FSlateBrush* >& OutBrushes ) const
{
	OutBrushes.Add( &Image );
}

const FAdvButtonSimpleStyle& FAdvButtonSimpleStyle::GetDefault()
{
	static FAdvButtonSimpleStyle Default;
	return Default;
}
