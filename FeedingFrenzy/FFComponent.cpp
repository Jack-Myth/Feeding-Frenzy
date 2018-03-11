#include "FFComponent.h"

void FFComponent::AttachComponentTo(class FFActor* TargetActor)
{
	this->TargetActor = TargetActor;
	OnComponentBegin(TargetActor);
	TimerHandle = FFTimerManager::Get().SetTimerPerTick(FFDelegate<void(float)>::CreateFromInstance(&FFComponent::OnComponentTick, this, _1PLCAE_HOLDER));
}

void FFComponent::RemoveComponent()
{
	FFTimerManager::Get().CancelTimer(TimerHandle);
	OnComponentEnd(TargetActor);
	this->TargetActor = nullptr;
}

FFComponent::FFComponent()
{

}

void FFComponent::OnComponentBegin(class FFActor* TargetActor)
{

}

void FFComponent::OnComponentEnd(class FFActor* LastActor)
{

}

void FFComponent::OnComponentTick(float SecondDetail)
{

}
