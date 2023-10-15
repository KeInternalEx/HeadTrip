#include "HtCore.h"

DEFINE_LOG_CATEGORY(HtCore);

#define LOCTEXT_NAMESPACE "FHtCore"

void FHtCore::StartupModule()
{
	UE_LOG(HtCore, Warning, TEXT("HtCore module has been loaded"));
}

void FHtCore::ShutdownModule()
{
	UE_LOG(HtCore, Warning, TEXT("HtCore module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHtCore, HtCore)