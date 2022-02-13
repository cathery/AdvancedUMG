// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvButtonBase.generated.h"

class SAdvButtonBase;

/**
 * The base for all Adv Buttons. It has all the functionality of a button without any of the visuals.
 * (Exception: The desired size override - I'm not sure if it will prove useful
 * but I've left it in the base class as all buttons share the property.)
 *
 * * Many Children
 * * Child Alignment
 * * Press, Click, and Hover events 
 */
UCLASS(Abstract)
class ADVANCEDUMG_API UAdvBaseButton : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** The type of mouse action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** The type of touch action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** The type of keyboard/gamepad button press action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool IsFocusable;

public:
	// I've put these inside the class to avoid global naming conflicts
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedEvent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressedEvent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonReleasedEvent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonHoverEvent);

	/** Called when the button is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Adv Button|Event")
	FOnButtonClickedEvent OnClicked;

	/** Called when the button is pressed */
	UPROPERTY(BlueprintAssignable, Category = "Adv Button|Event")
	FOnButtonPressedEvent OnPressed;

	/** Called when the button is released */
	UPROPERTY(BlueprintAssignable, Category = "Adv Button|Event")
	FOnButtonReleasedEvent OnReleased;

	/** Called when the button is hovered over */
	UPROPERTY(BlueprintAssignable, Category = "Adv Button|Event")
	FOnButtonHoverEvent OnHovered;

	/** Called when the button stops being hovered over */
	UPROPERTY(BlueprintAssignable, Category = "Adv Button|Event")
	FOnButtonHoverEvent OnUnhovered;

public:
	/**
	 * Returns true if the user is actively pressing the button.  Do not use this for detecting 'Clicks', use the OnClicked event instead.
	 *
	 * @return true if the user is actively pressing the button otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Adv Button")
	bool IsPressed() const;

	UFUNCTION(BlueprintCallable, Category = "Adv Button")
	void SetClickMethod(EButtonClickMethod::Type InClickMethod);

	UFUNCTION(BlueprintCallable, Category = "Adv Button")
	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);

	UFUNCTION(BlueprintCallable, Category = "Adv Button")
	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

public:
	UAdvBaseButton();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

protected:
	/** Handle the actual click event from slate and forward it on */
	FReply SlateHandleClicked();
	void   SlateHandlePressed();
	void   SlateHandleReleased();
	void   SlateHandleHovered();
	void   SlateHandleUnhovered();

protected:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override
	{
		return Content;
	}
#endif
	//~ End UWidget Interface

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override final;
	virtual void MakeSlateWidget() override;

	virtual TSharedPtr<SAdvButtonBase> GetSlateAdvButton() const;
};
