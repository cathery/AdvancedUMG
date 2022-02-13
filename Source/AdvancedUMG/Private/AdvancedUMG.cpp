// Copyright (c) 2022 cathery

#include "ADVancedUMG.h"

#include "AdvSlateStyle.h"

#define LOCTEXT_NAMESPACE "FAdvancedUMGModule"

void FAdvancedUMGModule::StartupModule()
{
	FAdvSlateStyle::Initialize();
}

void FAdvancedUMGModule::ShutdownModule()
{
	FAdvSlateStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAdvancedUMGModule, ADVancedUMG)