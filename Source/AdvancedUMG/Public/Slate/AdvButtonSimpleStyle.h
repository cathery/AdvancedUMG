// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "AdvButtonSimpleStyle.generated.h"

/**
 * Represents the appearance of an SAdvButtonSimple
 */
USTRUCT(BlueprintType)
struct ADVANCEDUMG_API FAdvButtonSimpleStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FAdvButtonSimpleStyle();

	virtual ~FAdvButtonSimpleStyle() override { }

	virtual void GetResources( TArray< const FSlateBrush* >& OutBrushes ) const override;

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };

	static const FAdvButtonSimpleStyle& GetDefault();

	/** Button appearance when the button is not hovered or pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush Image;
	FAdvButtonSimpleStyle& SetImage( const FSlateBrush& InImage ){ Image = InImage; return *this; }

	/** How much the button should nudge down when pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	float DownNudge;
	FAdvButtonSimpleStyle& SetDownNudge( float InDownNudge ){ DownNudge = InDownNudge; return *this; }


	/** Padding for the button's content */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FMargin ContentPadding;
	FAdvButtonSimpleStyle& SetPadding( const FMargin& InPadding ){ ContentPadding = InPadding; return *this; }

	/**
	 * The sound the button should play when pressed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound PressedSound;
	FAdvButtonSimpleStyle& SetPressedSound(const FSlateSound& InPressedSound ){ PressedSound = InPressedSound; return *this; }

	/**
	 * The sound the button should play when pressed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound ClickedSound;
	FAdvButtonSimpleStyle& SetClickedSound(const FSlateSound& InClickedSound ){ ClickedSound = InClickedSound; return *this; }

	/**
	 * The sound the button should play when hovered
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateSound HoveredSound;
	FAdvButtonSimpleStyle& SetHoveredSound(const FSlateSound& InHoveredSound ){ HoveredSound = InHoveredSound; return *this; }

	/**
	 * When the button is disabled, should it reflect that visually?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	bool bShowDisabledEffect;
	FAdvButtonSimpleStyle& SetShowDisabledEffect( bool bInShowDisabledEffect ){ bShowDisabledEffect = bInShowDisabledEffect; return *this; }

	/**
	 * Unlinks all colors in this style.
	 * @see FSlateColor::Unlink
	 */
	void UnlinkColors()
	{
		Image.UnlinkColors();
	}
};