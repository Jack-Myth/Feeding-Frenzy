#pragma once
#include "FFActor.h"

class FFControllableActor :public FFActor
{
	class FFController* CurController = nullptr;
public:
	virtual void Move(FFVector2D MoveDir);
	virtual void OnPossessed(class FFController* TargetController);
	virtual void OnUnPossessed(class FFController* LastController);
};