#include "FFControllers.h"

void FFController::Possess(FFControllableActor* TargetActor)
{
	ControlledActor->OnUnPossessed(this);
	ControlledActor = TargetActor;
	ControlledActor->OnPossessed(this);
}

FFControllableActor* FFController::GetControlledActor()
{
	return ControlledActor;
}

FFVector2D FFPlayerController::SightPoint;

FFPlayerController::FFPlayerController()
{
}

void FFPlayerController::Tick(float DetailTime)
{
	SightPoint = GetControlledActor()->GetLocation();
}

FFVector2D FFPlayerController::GetSightPoint()
{
	return SightPoint;
}
