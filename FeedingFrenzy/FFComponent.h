#pragma once
#include "FFTimerManager.h"

class FFComponent
{
	FFTimerHandle TimerHandle;
	class FFActor* TargetActor;
protected:
	friend class FFActor;
	void AttachComponentTo(class FFActor* TargetActor);
	void RemoveComponent();
public:
	FFComponent();
	virtual void OnComponentBegin(class FFActor* TargetActor);
	virtual void OnComponentEnd(class FFActor* LastActor);
	virtual void OnComponentTick(float SecondDetail);
};