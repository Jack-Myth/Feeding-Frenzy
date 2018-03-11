#pragma once
#include <vector>
#include "FFActor.h"
#include "FFMedia.h"
#include "json11.hpp"
#include "FFDebug.h"

class FFScene
{
	class FFUI;
	static std::vector<FFScene*> SceneCollection;
	std::vector<FFActor*> SceneActors;
	std::vector<FFUI*> SceneUI;
	FFBitmap SceneBackground;
	std::vector<std::pair<FFDelegate<FFActor*(json11::Json::object)>, json11::Json::object>> DynamicActor;
	class FFGameMode* GameMode = NULL;
	FFVector2D Border = 1024;
	FFVector2D PositionOffset = 0;
	int ZOrder = -1;
protected:
	virtual void RenderScene();
public:
	FFScene();

	~FFScene();

	FFVector2D GetBorder();
	void SetBorder(const FFVector2D& NewBorder);
	FFVector2D GetPositionOffset();
	void SetPositionOffset(const FFVector2D& NewPositionOffset);
	static void LoadScene(const json11::Json& Args);

	static std::vector<FFScene*> GetSceneCollection();

	void SetBackgroundImage(const FFBitmap& NewBackground);
	void SetBackground(std::string FilePath);

	template<typename T>
	T* SpawnActor(D2D1_POINT_2F Location, D2D1_POINT_2F Scale);

	void AssignActor(FFActor* Actor);
};

template<typename T>
T* FFScene::SpawnActor(D2D1_POINT_2F Location, D2D1_POINT_2F Scale)
{
	T* tmpActorRef = new T;
	tmpActorRef->SetLocation(Location);
	tmpActorRef->SetScale(Scale);
	SceneActors.push_back(tmpActorRef);
	tmpActorRef->BeginPlay();
	return tmpActorRef;
}
