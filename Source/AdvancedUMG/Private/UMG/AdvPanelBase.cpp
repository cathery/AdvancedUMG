// Copyright (c) 2022 cathery

#include "UMG/AdvPanelBase.h"

#include "UMG/AdvPanelSlot.h"

UAdvPanelBase::UAdvPanelBase()
{
	bIsVariable = false;
	Visibility  = ESlateVisibility::SelfHitTestInvisible;
}

UClass* UAdvPanelBase::GetSlotClass() const
{
	return UAdvPanelSlot::StaticClass();
}

void UAdvPanelBase::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (GetSlateAdvPanel())
	{
		CastChecked<UAdvPanelSlot>(InSlot)->BuildSlot(GetSlateAdvPanel().ToSharedRef());
	}
}

void UAdvPanelBase::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (GetSlateAdvPanel() && InSlot->Content)
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			GetSlateAdvPanel()->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UAdvPanelBase::RebuildWidget()
{
	MakeSlateWidget();

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UAdvPanelSlot* TypedSlot = Cast<UAdvPanelSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(GetSlateAdvPanel().ToSharedRef());
		}
	}

	return GetSlateAdvPanel().ToSharedRef();
}

TSharedPtr<SAdvPanel> UAdvPanelBase::GetSlateAdvPanel() const
{
	return nullptr;
}

void UAdvPanelBase::MakeSlateWidget()
{
	BindSlateWidgetEvents();
}

void UAdvPanelBase::BindSlateWidgetEvents()
{
	GetSlateAdvPanel()->SetOnMouseEnter(FNoReplyPointerEventHandler::CreateUObject(this, &ThisClass::OnSlateMouseEnter));
	GetSlateAdvPanel()->SetOnMouseMove(FPointerEventHandler::CreateUObject(this, &ThisClass::OnSlateMouseMove));
	GetSlateAdvPanel()->AdvMouseWheelEvent.BindUObject(this, &ThisClass::OnSlateAdvMouseWheelEvent);
	GetSlateAdvPanel()->SetOnMouseButtonDown(FPointerEventHandler::CreateUObject(this, &ThisClass::OnSlateMouseButtonDown));
	GetSlateAdvPanel()->SetOnMouseButtonUp(FPointerEventHandler::CreateUObject(this, &ThisClass::OnSlateMouseButtonUp));
	GetSlateAdvPanel()->SetOnMouseLeave(FSimpleNoReplyPointerEventHandler::CreateUObject(this, &ThisClass::OnSlateMouseLeave));
}

void UAdvPanelBase::OnSlateMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MouseEnterEvent.ExecuteIfBound(InGeometry, InMouseEvent);
}

FReply UAdvPanelBase::OnSlateMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Probably performance loss due condition

	return MouseMoveEvent.ExecuteIfBound(InGeometry, InMouseEvent) ? FReply::Handled() : FReply::Unhandled();
}

FReply UAdvPanelBase::OnSlateMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Probably performance loss due condition

	return MouseButtonDownEvent.ExecuteIfBound(InGeometry, InMouseEvent) ? FReply::Handled() : FReply::Unhandled();
}

FReply UAdvPanelBase::OnSlateMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Probably performance loss due condition

	return MouseButtonUpEvent.ExecuteIfBound(InGeometry, InMouseEvent) ? FReply::Handled() : FReply::Unhandled();
}

void UAdvPanelBase::OnSlateAdvMouseWheelEvent(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MouseWheelEvent.ExecuteIfBound(InGeometry, InMouseEvent);
}

void UAdvPanelBase::OnSlateMouseLeave(const FPointerEvent& InMouseEvent)
{
	MouseLeaveEvent.ExecuteIfBound(InMouseEvent);
}

#if WITH_EDITOR

const FText UAdvPanelBase::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "AdvWidgets", "Advanced Widgets");
}

#endif
