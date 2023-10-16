// Copyright (c) 2022 cathery

#include "UMG/AdvBorder.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvBorder.h"

UAdvBorder::UAdvBorder()
{
	bIsVariable = true;
	SetVisibilityInternal(ESlateVisibility::Visible);
	Brush.Margin = FMargin(0.3333f);
	Brush.DrawAs = ESlateBrushDrawType::Border;
	ImageScale = 1.0f;
}

void UAdvBorder::SetBrush(const FSlateBrush& InBrush)
{
	Brush = InBrush;

	if (MyBorder)
	{
		MyBorder->SetImage(InBrush);
	}
}

void UAdvBorder::SetImageScale(float InImageScale)
{
	ImageScale = InImageScale;
	if (MyBorder)
	{
		MyBorder->SetImageScale(InImageScale);
	}
}

void UAdvBorder::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyBorder)
	{
		MyBorder->SetImage(Brush);
		MyBorder->SetImageScale(ImageScale);
	}
}

void UAdvBorder::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyBorder.Reset();
}

#if WITH_EDITOR

void UAdvBorder::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static bool IsReentrant = false;

	if ( !IsReentrant )
	{
		IsReentrant = true;

		if ( PropertyChangedEvent.Property )
		{
			static const FName DrawTypeName("DrawAs");

			if (PropertyChangedEvent.Property->GetFName() == DrawTypeName)
			{
				Brush.DrawAs = ESlateBrushDrawType::Border;
			}
		}

		IsReentrant = false;
	}
}

#endif

TSharedPtr<SAdvPanel> UAdvBorder::GetSlateAdvPanel() const
{
	return MyBorder;
}

void UAdvBorder::MakeSlateWidget()
{
	MyBorder = SNew(SAdvBorder);
}
