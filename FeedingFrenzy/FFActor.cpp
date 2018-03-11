#include "FFActor.h"

FFTransform FFActor::GetActorTransform()
{
	return ActorTransform;
}

void FFActor::SetActorTransform(const FFTransform& NewActorTransform)
{
	ActorTransform = NewActorTransform;
}

int FFActor::GetZOrder()
{
	return ZOrder;
}

void FFActor::SetZOrder(int NewZOrder)
{
	ZOrder = NewZOrder;
}

std::string FFActor::GetType()
{
	return std::string();
}

bool FFActor::IsUI()
{
	return false;
}

FFActor::FFActor()
{
	TickTimerHandle = FFTimerManager::Get().SetTimerPerTick(FFDelegate<void(float)>::CreateFromInstance(&FFActor::Tick, this, _1PLCAE_HOLDER));
	FFRenderManager::Get().AddRenderDelegate(new FFRenderInfo(FFDelegate<void()>::CreateFromInstance(&FFActor::Render, this), &ZOrder));
}

void FFActor::SetLocation(const FFVector2D& Location)
{
	ActorTransform.Location = Location;
}

FFVector2D FFActor::GetLocation()
{
	return ActorTransform.Location;
}

void FFActor::AddLocation(const FFVector2D& Offset)
{
	ActorTransform.Location += Offset;
}

void FFActor::SetScale(const FFVector2D& Scale)
{
	ActorTransform.Scale = Scale;
}

FFVector2D FFActor::GetScale()
{
	return ActorTransform.Scale;
}

FFActor::~FFActor()
{
	FFTimerManager::Get().CancelTimer(TickTimerHandle);
}

void FFActor::AddComponent(FFComponent* ActorComponent)
{
	Components.push_back(ActorComponent);
}

void FFActor::RemoveComponent(const FFComponent*& ActorComponent)
{
	Components.erase(std::find(Components.begin(), Components.end(), ActorComponent));
}

