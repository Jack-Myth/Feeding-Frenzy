#include "FF2DOperations.h"
#include "FFReflectionManager.h"
#include "FFScene.h"

//FileName
DECLARE_OPERATION(LoadModule)
{
	json11::Json FileNames = Args.object_items().find("FileName")->second;
	if (FileNames.is_array())
	{
		for (auto it = FileNames.array_items().begin(); it < FileNames.array_items().end(); ++it)
		{
			if (!FFReflectionManager::Get().LoadLibrary(it->string_value()))
				return false;
		}
		return true;
	}
	else
	{
		if (FFReflectionManager::Get().UnloadLibrary(FileNames.string_value()))
			return true;
		return false;
	}
}

DECLARE_OPERATION(UnloadModule)
{
	json11::Json FileNames = Args.object_items().find("FileName")->second;
	if (FileNames.is_array())
	{
		for (auto it = FileNames.array_items().begin(); it < FileNames.array_items().end(); ++it)
		{
			if (!FFReflectionManager::Get().UnloadLibrary(it->string_value()))
				return false;
		}
		return true;
	}
	else
	{
		if (!FFReflectionManager::Get().UnloadLibrary(FileNames.string_value()))
			return false;
		return true;
	}
}

//FileName
DECLARE_OPERATION(LoadJson)
{
	json11::Json FileNames = Args.object_items().find("FileName")->second;
	if (FileNames.is_array())
	{
		for (auto it = FileNames.array_items().begin(); it < FileNames.array_items().end(); ++it)
		{
			FFReflectionManager::Get().ExecuteJsonFile(it->string_value());
		}
	}
	else
	{
		FFReflectionManager::Get().ExecuteJsonFile(FileNames.string_value());
	}
	return true;
}

DECLARE_OPERATION(Define)
{
	if (auto DefineNameIt = Args.object_items().find("Name"); DefineNameIt != Args.object_items().end())
	{
		if (auto PropertiesIt = Args.object_items().find("Properties"); PropertiesIt != Args.object_items().end())
		{
			FFReflectionManager::Get().SetDefine(DefineNameIt->second.string_value(), PropertiesIt->second);
		}
	}
	return true;
}

DECLARE_OPERATION(Exec)
{
	FFReflectionManager::Get().ExecuteJson(Args);
	return true;
}

DECLARE_OPERATION(SpawnActor)
{
	auto ActorTypeIt = Args.object_items().find("Type");
	if (ActorTypeIt == Args.object_items().end())
		return false;
	json11::Json::object tmpJsonObject;
	auto DefineIt = Args.object_items().find("Defined");
	if (DefineIt != Args.object_items().end())
	{
		if (auto JsonData = FFReflectionManager::Get().GetDefined(DefineIt->second.string_value()); JsonData == json11::Json())
			tmpJsonObject = JsonData.object_items();
	}
	auto PropertiesIt = Args.object_items().find("Properties");
	if (PropertiesIt != Args.object_items().end())
	{
		for (auto it = PropertiesIt->second.object_items().begin(); it != PropertiesIt->second.object_items().end(); ++it)
		{
			tmpJsonObject.insert_or_assign(it->first, it->second);
		}
	}
	FFActor* pActor = FFReflectionManager::Get().GetActorTypeConstructor(ActorTypeIt->second.string_value()).GetStdFunction()(tmpJsonObject);
	auto SceneC = FFScene::GetSceneCollection();
	if (SceneC.empty())
	{
		SceneC[0]->AssignActor(pActor);
	}
	return true;
}