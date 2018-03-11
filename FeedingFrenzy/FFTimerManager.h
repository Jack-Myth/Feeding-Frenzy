#pragma once
#include <wtypes.h>
#include "FFDelegate.h"
#include <vector>

class FFTimerManager  //Global Timer Manager
{
	static FFTimerManager* TimerManager;
	friend void DoLogic();
	struct TimerInfo
	{
		friend FFTimerManager;
	public:
		int TimeRemain = 0;  //ms
		bool ShouldLoop = false;
		int Interval;
		FFDelegate<void()> TimerDelegate;
	};
	struct TimerInfoPerTick
	{
		FFDelegate<void(float)> TimerDelegate;
	};
	std::vector<TimerInfo*> Timers;
	std::vector<TimerInfoPerTick*> TimersPerTick;
public:
	struct FFTimerHandle
	{
		bool isPerTickTimer;
		union Info
		{
			TimerInfo* pTimerInfo;
			TimerInfoPerTick* pTimerInfoPerTick;
		} TimerHandleInfo;
	};
	static FFTimerManager & Get();
private:
	void Tick(int DetailTime_ms);
	FFTimerHandle ConstructTimerHandle(bool IsPerTickTimer, void* pTimerInfo);
public:
	//Use Delegate, Easy To Bind
	//Return Value:Timer ID
	FFTimerHandle SetTimer(FFDelegate<void()> TimerDelegate, float firstDelay, bool IsLoop = false, float Interval = 0.f);
	FFTimerHandle SetTimerPerTick(FFDelegate<void(float)> TimerDelegate);
	bool CancelTimer(const FFTimerHandle& TimerHandle);
};
typedef FFTimerManager::FFTimerHandle FFTimerHandle;
