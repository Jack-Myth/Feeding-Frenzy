#include "FFScene.h"

void FFScene::RenderScene()
{

}

FFScene::FFScene()
{
	FFRenderManager::Get().AddRenderDelegate(new FFRenderInfo(FFDelegate<void()>::CreateFromInstance(&FFScene::RenderScene, this), &ZOrder));
	SceneCollection.push_back(this);
}

FFScene::~FFScene()
{
	SceneCollection.erase(std::find(SceneCollection.begin(), SceneCollection.end(), this));
}

FFVector2D FFScene::GetBorder()
{
	return Border;
}

void FFScene::SetBorder(const FFVector2D& NewBorder)
{
	Border = NewBorder;
}

FFVector2D FFScene::GetPositionOffset()
{
	return PositionOffset;
}

void FFScene::SetPositionOffset(const FFVector2D& NewPositionOffset)
{
	PositionOffset = NewPositionOffset;
}

void FFScene::LoadScene(const json11::Json& Args)
{
	for (FFScene*& SceneElement : SceneCollection)
	{
		delete SceneElement;
	}
	SceneCollection.clear();
	SceneCollection.push_back(new FFScene());
	auto JsonObjectData = Args.object_items();
	if (auto BGIt = JsonObjectData.find("Background"); BGIt != JsonObjectData.end())
		SceneCollection[0]->SetBackgroundImage(BGIt->second.string_value());
	/*if (auto DynamicActorsIt = JsonObjectData.find("DynamicActors");DynamicActorsIt!=JsonObjectData.end())
	{
	for (const auto & DynamicActorIt : DynamicActorsIt->second.object_items())
	{

	SceneCollection[0]->DynamicActor.push_back(
	std::pair<FFDelegate<FFActor*(json11::Json::object)>,json11::Json::object>(
	FFReflectionManager::Get().GetActorTypeConstructor(DynamicActorIt.first), DynamicActorIt.second.object_items()));
	}
	}*/
}

std::vector<FFScene*> FFScene::GetSceneCollection()
{
	return SceneCollection;
}

void FFScene::SetBackgroundImage(const FFBitmap& NewBackground)
{
	SceneBackground = NewBackground;
}

void FFScene::SetBackground(std::string FilePath)
{
	SceneBackground.LoadResourceFrom(FilePath);
}

void FFScene::AssignActor(FFActor* Actor)
{
	if (std::find(SceneActors.begin(), SceneActors.end(), Actor) == SceneActors.end())
	{
		if (Actor->IsUI())
			SceneUI.push_back((FFUI*)Actor);
		else
			SceneActors.push_back(Actor);
	}
	else
		debug << "Scene Alreay Add an \"" << Actor->GetType() << "\" Actor";
}

std::vector<FFScene*> FFScene::SceneCollection;