// Copyright (c) 2022 cathery

#pragma once

#include "Styling/SlateStyle.h"

class FAdvSlateStyle: public FSlateStyleSet
{
public:
	FAdvSlateStyle(const FName& InStyleSetName);

	static void Initialize();
	static void Shutdown();

	/** @return the singleton instance. */
	static const ISlateStyle& Get()
	{
		return *Instance.Get();
	}

private:
	static TSharedPtr<ISlateStyle> Instance;
};
