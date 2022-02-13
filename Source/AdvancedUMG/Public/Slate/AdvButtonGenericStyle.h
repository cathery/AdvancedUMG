// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "AdvButtonGenericStyle.generated.h"

/**
 * Represents the appearance of an SAdvButtonGeneric
 */
USTRUCT(BlueprintType)
struct ADVANCEDUMG_API FAdvButtonGenericStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FAdvButtonGenericStyle();

	virtual ~FAdvButtonGenericStyle() override { }

	virtual void GetResources( TArray< const FSlateBrush* >& OutBrushes ) const override;

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };

	static const FAdvButtonGenericStyle& GetDefault();

	/** Button appearance when the button is not hovered or pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush Normal;
	FAdvButtonGenericStyle& SetNormal( const FSlateBrush& InNormal ){ Normal = InNormal; return *this; }

	/** Button appearance when hovered */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush Hovered;
	FAdvButtonGenericStyle& SetHovered( const FSlateBrush& InHovered){ Hovered = InHovered; return *this; }

	/** Button appearance when pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush Pressed;
	FAdvButtonGenericStyle& SetPressed( const FSlateBrush& InPressed ){ Pressed = InPressed; return *this; }

	/** Button appearance when disabled, by default this is set to an invalid resource when that is the case default disabled drawing is used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush Disabled;
	FAdvButtonGenericStyle& SetDisabled( const FSlateBrush& InDisabled ){ Disabled = InDisabled; return *this; }

	/**
	 * Padding that accounts for the border in the button's background image.
	 * When this is applied, the content of the button should appear flush
	 * with the button's border. Use this padding when the button is not pressed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FMargin NormalPadding;
	FAdvButtonGenericStyle& SetNormalPadding( const FMargin& InNormalPadding){ NormalPadding = InNormalPadding; return *this; }

	/**
	 * Same as NormalPadding but used when the button is pressed. Allows for moving the content to match
	 * any "movement" in the button's border image.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FMargin PressedPadding;
	FAdvButtonGenericStyle& SetPressedPadding( const FMargin& InPressedPadding){ PressedPadding = InPressedPadding; return *this; }

	/**
	 * The sound the button should play when pressed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound PressedSound;
	FAdvButtonGenericStyle& SetPressedSound(const FSlateSound& InPressedSound ){ PressedSound = InPressedSound; return *this; }

	/**
	 * The sound the button should play when pressed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound ClickedSound;
	FAdvButtonGenericStyle& SetClickedSound(const FSlateSound& InClickedSound ){ ClickedSound = InClickedSound; return *this; }

	/**
	 * The sound the button should play when hovered
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound HoveredSound;
	FAdvButtonGenericStyle& SetHoveredSound(const FSlateSound& InHoveredSound ){ HoveredSound = InHoveredSound; return *this; }

	/**
	 * Unlinks all colors in this style.
	 * @see FSlateColor::Unlink
	 */
	void UnlinkColors()
	{
		Normal.UnlinkColors();
		Hovered.UnlinkColors();
		Pressed.UnlinkColors();
		Disabled.UnlinkColors();
	}
};