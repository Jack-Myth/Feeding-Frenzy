#include "FFTimerManager.h"

FFTimerManager & FFTimerManager::Get()
{
	if (!TimerManager)
	{
		TimerManager = new FFTimerManager();
	}
	return *TimerManager;
}

void FFTimerManager::Tick(int DetailTime_ms)
{
	for (auto it = TimersPerTick.begin(); it < TimersPerTick.end(); ++it)
	{
		(*it)->TimerDelegate.GetStdFunction()(DetailTime_ms / 1000.f);
	}
	for (auto it = Timers.begin(); it < Timers.end(); ++it)
	{
		(*it)->TimeRemain -= DetailTime_ms;
		if ((*it)->TimeRemain <= 0)
		{
			(*it)->TimerDelegate.GetStdFunction()();
			if ((*it)->ShouldLoop)
			{
				(*it)->TimeRemain += (*it)->Interval;
			}
			else
			{
				it = Timers.erase(it);
			}
		}
	}
}

FFTimerHandle FFTimerManager::SetTimer(FFDelegate<void()> TimerDelegate, float firstDelay, bool IsLoop /*= false*/, float Interval /*= 0.f*/)
{
	TimerInfo* tmpTimerInfo = new TimerInfo;
	tmpTimerInfo->ShouldLoop = IsLoop;
	tmpTimerInfo->TimeRemain = (int)(firstDelay * 1000);
	tmpTimerInfo->TimerDelegate = TimerDelegate;
	tmpTimerInfo->Interval = (int)(Interval * 1000);
	Timers.push_back(tmpTimerInfo);
	return ConstructTimerHandle(false, tmpTimerInfo);
}

FFTimerHandle FFTimerManager::SetTimerPerTick(FFDelegate<void(float)> TimerDelegate)
{
	TimerInfoPerTick* tmpTimerInfoPerTick = new TimerInfoPerTick;
	tmpTimerInfoPerTick->TimerDelegate = TimerDelegate;
	TimersPerTick.push_back(tmpTimerInfoPerTick);
	return ConstructTimerHandle(true, tmpTimerInfoPerTick);
}

bool FFTimerManager::CancelTimer(const FFTimerHandle& TimerHandle)
{
	if (TimerHandle.isPerTickTimer)
	{
		auto It = std::find(TimersPerTick.begin(), TimersPerTick.end(), TimerHandle.TimerHandleInfo.pTimerInfoPerTick);
		if (It != TimersPerTick.end())
		{
			TimersPerTick.erase(It);
			return true;
		}
		else
			return false;
	}
	else
	{
		auto It = std::find(Timers.begin(), Timers.end(), TimerHandle.TimerHandleInfo.pTimerInfo);
		if (It != Timers.end())
		{
			Timers.erase(It);
			return true;
		}
		else
			return false;
	}
}

FFTimerHandle FFTimerManager::ConstructTimerHandle(bool IsPerTickTimer, void* pTimerInfo)
{
	FFTimerHandle tmpTimerHandle;
	tmpTimerHandle.isPerTickTimer = IsPerTickTimer;
	if (IsPerTickTimer)
		tmpTimerHandle.TimerHandleInfo.pTimerInfoPerTick = (TimerInfoPerTick*)pTimerInfo;
	else
		tmpTimerHandle.TimerHandleInfo.pTimerInfo = (TimerInfo*)pTimerInfo;
	return tmpTimerHandle;
}

FFTimerManager* FFTimerManager::TimerManager = NULL;