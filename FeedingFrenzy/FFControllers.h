#pragma once
#include "FFActor.h"
#include "FFControllableActor.h"

class FFController :public FFActor
{
	FFControllableActor* ControlledActor;
public:
	virtual void Possess(FFControllableActor* TargetActor);
	FFControllableActor* GetControlledActor();
};

class FFPlayerController :public FFController
{
	static FFVector2D SightPoint;
public:
	FFPlayerController();
	virtual void Tick(float DetailTime) override;
	static FFVector2D GetSightPoint();
};

class FFAIController :FFController
{

};