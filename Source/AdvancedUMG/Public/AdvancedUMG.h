// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

class FAdvancedUMGModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
