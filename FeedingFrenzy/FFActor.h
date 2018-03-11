#pragma once
#include "GlobalMacro.h"
#include "FFType.h"
#include "FFTimerManager.h"
#include <vector>
#include "FFRenderManager.h"
#include "FFComponent.h"

class FFActor
{
	friend class FFScene;
	FFScene* OuterScene = NULL;
	FFTransform ActorTransform;
	int ZOrder = 0;
	FFTimerHandle TickTimerHandle;
	std::vector<FFComponent*> Components;
protected:
	virtual void Render() {}
public:
	FFTransform GetActorTransform();
	void SetActorTransform(const FFTransform& NewActorTransform);
	int GetZOrder();
	void SetZOrder(int NewZOrder);
	virtual void BeginPlay() {}
	virtual void Tick(float DetailTime) {}
	virtual void EndPlay() {}
	virtual std::string GetType();
	virtual bool IsUI();
	FFActor();
	void SetLocation(const FFVector2D& Location);
	FFVector2D GetLocation();
	void AddLocation(const FFVector2D& Offset);
	void SetScale(const FFVector2D& Scale);
	FFVector2D GetScale();
	virtual ~FFActor();
	void AddComponent(FFComponent* ActorComponent);
	void RemoveComponent(const FFComponent*& ActorComponent);
};