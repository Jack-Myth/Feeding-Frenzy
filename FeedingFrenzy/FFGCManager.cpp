#include "FFGCManager.h"
#include "FFDelegate.h"
#include "FFTimerManager.h"
#include "GlobalMacro.h"
#include "FFMedia.h"

FFGCManager::FFGCManager()
{
	FFTimerManager::Get().SetTimer(FFDelegate<void()>::CreateFromLambda([this]()
																		{
																			StartGC();
																		}), GC_INTERVAL, true, GC_INTERVAL);
}

void FFGCManager::StartGC()
{
	FFResource::StartGC();
}

FFGCManager& FFGCManager::Get()
{
	if (!GCManager)
	{
		GCManager = new FFGCManager();
	}
	return *GCManager;
}

FFGCManager* FFGCManager::GCManager=NULL;