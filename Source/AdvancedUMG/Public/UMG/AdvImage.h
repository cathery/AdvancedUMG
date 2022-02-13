// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Slate/SlateTextureAtlasInterface.h"

#include "UMG/AdvPanelBase.h"

#include "AdvImage.generated.h"

class USlateBrushAsset;

/**
 * Image that lets you place children inside.
 *
 * * Many Children
 * * Child Alignment
 */
UCLASS(meta = (DisplayName = "Adv Image"))
class ADVANCEDUMG_API UAdvImage : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** Image to draw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	/** A bindable delegate for the Image. */
	UPROPERTY()
	FGetSlateBrush BrushDelegate;

	/** Color and opacity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true"))
	FLinearColor ColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor ColorAndOpacityDelegate;

	/** Flips the image if the localization's flow direction is RightToLeft */
	UPROPERTY(EditAnywhere, Category = "Localization")
	bool bFlipForRightToLeftFlowDirection;

public:
	UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseButtonDownEvent;

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushSize(FVector2D DesiredSize);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushTintColor(FSlateColor TintColor);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushResourceObject(UObject* ResourceObject);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrush(const FSlateBrush& InBrush);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromAsset(USlateBrushAsset* Asset);

	/**
	* Sets the Brush to the specified Texture.
	*
	* @param Texture Texture to use to set on Brush.
	* @param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);

	/**
	* Sets the Brush to the specified Atlas Region.
	*
	* @param AtlasRegion Region of the Atlas to use to set on Brush.
	* @param bMatchSize If true, image will change its size to atlas region size. If false, atlas region will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize = false);

	/**
	* Sets the Brush to the specified Dynamic Texture.
	*
	* @param Texture Dynamic Texture to use to set on Brush.
	* @param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromMaterial(UMaterialInterface* Material);

	/**
	* Sets the Brush to the specified Soft Texture.
	*
	* @param SoftTexture Soft Texture to use to set on Brush.
	* @param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	UMaterialInstanceDynamic* GetDynamicMaterial();

public:
	UAdvImage();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	/** Translates the bound brush data and assigns it to the cached brush used by this widget. */
	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

	// Called when we need to stream in content.
	void         RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, TFunction<void()>&& Callback);
	virtual void RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, FStreamableDelegate DelegateToCall);

	// Called when we need to abort the texture being streamed in.
	virtual void CancelImageStreaming();

	// Called when the image streaming starts, after the other one was cancelled.
	virtual void OnImageStreamingStarted(TSoftObjectPtr<UObject> SoftObject);

	// Called when the image streaming completes.
	virtual void OnImageStreamingComplete(TSoftObjectPtr<UObject> LoadedSoftObject);

	//
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const;

protected:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override
	{
		return Content;
	}
#endif
	//~ End UWidget Interface

protected:
	TSharedPtr<class SAdvImage> MyImage;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;

protected:
	TSharedPtr<FStreamableHandle> StreamingHandle;
	FSoftObjectPath StreamingObjectPath;

protected:
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
};
