// Copyright (c) 2022 cathery

#include "UMG/AdvImage.h"

#include "Engine/AssetManager.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Slate/SlateBrushAsset.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvImage.h"

void UAdvImage::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyImage)
	{
		MyImage->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UAdvImage::SetOpacity(float InOpacity)
{
	ColorAndOpacity.A = InOpacity;
	if (MyImage)
	{
		MyImage->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UAdvImage::SetBrushSize(FVector2D DesiredSize)
{
	if (Brush.ImageSize != DesiredSize)
	{
		Brush.ImageSize = DesiredSize;

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushTintColor(FSlateColor TintColor)
{
	if (Brush.TintColor != TintColor)
	{
		Brush.TintColor = TintColor;

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushResourceObject(UObject* ResourceObject)
{
	if (Brush.GetResourceObject() != ResourceObject)
	{
		Brush.SetResourceObject(ResourceObject);

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrush(const FSlateBrush& InBrush)
{
	if (Brush != InBrush)
	{
		Brush = InBrush;

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromAsset(USlateBrushAsset* Asset)
{
	if (!Asset || Brush != Asset->Brush)
	{
		CancelImageStreaming();
		Brush = Asset ? Asset->Brush : FSlateBrush();

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize)
{
	CancelImageStreaming();

	if (Brush.GetResourceObject() != Texture)
	{
		Brush.SetResourceObject(Texture);

		if (Texture) // Since this texture is used as UI, don't allow it affected by budget.
		{
			Texture->bForceMiplevelsToBeResident = true;
			Texture->bIgnoreStreamingMipBias     = true;
		}

		if (bMatchSize)
		{
			if (Texture)
			{
				Brush.ImageSize.X = Texture->GetSizeX();
				Brush.ImageSize.Y = Texture->GetSizeY();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize)
{
	if (Brush.GetResourceObject() != AtlasRegion.GetObject())
	{
		CancelImageStreaming();
		Brush.SetResourceObject(AtlasRegion.GetObject());

		if (bMatchSize)
		{
			if (AtlasRegion)
			{
				const FSlateAtlasData AtlasData = AtlasRegion->GetSlateAtlasData();
				Brush.ImageSize                 = AtlasData.GetSourceDimensions();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize)
{
	if (Brush.GetResourceObject() != Texture)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Texture);

		if (bMatchSize && Texture)
		{
			Brush.ImageSize.X = Texture->SizeX;
			Brush.ImageSize.Y = Texture->SizeY;
		}

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromMaterial(UMaterialInterface* Material)
{
	if (Brush.GetResourceObject() != Material)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Material);

		//TODO UMG Check if the material can be used with the UI

		if (MyImage)
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UAdvImage::SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize)
{
	TWeakObjectPtr<UAdvImage> WeakThis(this); // using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftTexture,
		[WeakThis, SoftTexture, bMatchSize]()
		{
			if (UAdvImage* StrongThis = WeakThis.Get())
			{
				ensureMsgf(SoftTexture.Get(), TEXT("Failed to load %s"), *SoftTexture.ToSoftObjectPath().ToString());
				StrongThis->SetBrushFromTexture(SoftTexture.Get(), bMatchSize);
			}
		}
	);
}

void UAdvImage::SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial)
{
	TWeakObjectPtr<UAdvImage> WeakThis(this); // using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftMaterial,
		[WeakThis, SoftMaterial]()
		{
			if (UAdvImage* StrongThis = WeakThis.Get())
			{
				ensureMsgf(SoftMaterial.Get(), TEXT("Failed to load %s"), *SoftMaterial.ToSoftObjectPath().ToString());
				StrongThis->SetBrushFromMaterial(SoftMaterial.Get());
			}
		}
	);
}

UMaterialInstanceDynamic* UAdvImage::GetDynamicMaterial()
{
	UObject*            Resource = Brush.GetResourceObject();
	UMaterialInterface* Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Brush.SetResourceObject(DynamicMaterial);

			if (MyImage)
			{
				MyImage->SetImage(&Brush);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?

	return nullptr;
}

UAdvImage::UAdvImage()
	: ColorAndOpacity(FLinearColor::White)
{
	bIsVariable = true;
	Visibility  = ESlateVisibility::Visible;
}

void UAdvImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const TAttribute<FSlateColor>        ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	const TAttribute<const FSlateBrush*> ImageBinding           = OPTIONAL_BINDING_CONVERT(FSlateBrush, Brush, const FSlateBrush*, ConvertImage);

	if (MyImage)
	{
		MyImage->SetImage(ImageBinding);
		MyImage->SetColorAndOpacity(ColorAndOpacityBinding);
		MyImage->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
	}
}

void UAdvImage::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImage.Reset();
}

const FSlateBrush* UAdvImage::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UAdvImage* MutableThis = const_cast<UAdvImage*>(this);
	MutableThis->Brush    = InImageAsset.Get();

	return &Brush;
}

void UAdvImage::RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, TFunction<void()>&& Callback)
{
	RequestAsyncLoad(SoftObject, FStreamableDelegate::CreateLambda(MoveTemp(Callback)));
}

void UAdvImage::RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, FStreamableDelegate DelegateToCall)
{
	CancelImageStreaming();

	if (UObject* StrongObject = SoftObject.Get())
	{
		DelegateToCall.ExecuteIfBound();
		return; // No streaming was needed, complete immediately.
	}

	OnImageStreamingStarted(SoftObject);

	TWeakObjectPtr<UAdvImage> WeakThis(this);
	StreamingObjectPath = SoftObject.ToSoftObjectPath();
	StreamingHandle     = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StreamingObjectPath,
		[WeakThis, DelegateToCall, SoftObject]()
		{
			if (UAdvImage* StrongThis = WeakThis.Get())
			{
				// If the object paths don't match, then this delegate was interrupted, but had already been queued for a callback
				// so ignore everything and abort.
				if (StrongThis->StreamingObjectPath != SoftObject.ToSoftObjectPath())
				{
					return; // Abort!
				}

				// Call the delegate to do whatever is needed, probably set the new image.
				DelegateToCall.ExecuteIfBound();

				// Note that the streaming has completed.
				StrongThis->OnImageStreamingComplete(SoftObject);
			}
		},
		FStreamableManager::AsyncLoadHighPriority);
}

void UAdvImage::CancelImageStreaming()
{
	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}

	StreamingObjectPath.Reset();
}

void UAdvImage::OnImageStreamingStarted(TSoftObjectPtr<UObject> SoftObject)
{
	// No-Op
}

void UAdvImage::OnImageStreamingComplete(TSoftObjectPtr<UObject> LoadedSoftObject)
{
	// No-Op
}

FReply UAdvImage::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const
{
	if (OnMouseButtonDownEvent.IsBound())
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

TSharedPtr<SAdvPanel> UAdvImage::GetSlateAdvPanel() const
{
	return MyImage;
}

void UAdvImage::MakeSlateWidget()
{
	MyImage = SNew(SAdvImage)
	.FlipForRightToLeftFlowDirection(bFlipForRightToLeftFlowDirection);
}
