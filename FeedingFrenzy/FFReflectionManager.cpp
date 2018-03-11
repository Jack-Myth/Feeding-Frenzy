#include "FFReflectionManager.h"
#include "GlobalMacro.h"
#include "FFData.h"

FFReflectionManager & FFReflectionManager::Get()
{
	if (!ReflectManager)
		ReflectManager = new FFReflectionManager();
	return *ReflectManager;
}

const std::string& FFReflectionManager::__ToReflect(const std::string& RefrectName, const FFDelegate<class FFActor*(json11::Json::object)>& ReflectDelegate)
{
	FFActorReflectFunctions.insert_or_assign(RefrectName, ReflectDelegate);
	return RefrectName;
}

FFDelegate<class FFActor*(json11::Json::object)> FFReflectionManager::GetActorTypeConstructor(const std::string& ReflectName)
{
	if (auto it = FFActorReflectFunctions.find(ReflectName); it != FFActorReflectFunctions.end())
	{
		return it->second;
	}
	else
	{
		return FFDelegate<class FFActor*(json11::Json::object)>();
	}
}

std::map<std::string, HMODULE> FFReflectionManager::GetModules()
{
	return ModuleMap;
}

bool FFReflectionManager::LoadLibrary(std::string LibraryPath)
{
	auto tmpModule = LoadLibraryA(LibraryPath.c_str());
	if (tmpModule)
	{
		ModuleMap.insert_or_assign(LibraryPath, tmpModule);
		return true;
	}
	return false;
}

bool FFReflectionManager::UnloadLibrary(std::string LibraryPath)
{
	if (auto ModuleIt = ModuleMap.find(LibraryPath); ModuleIt != ModuleMap.end())
	{
		if (!FreeLibrary(ModuleIt->second))
			return false;
		ModuleMap.erase(ModuleIt);
	}
	return false;
}

void FFReflectionManager::SetDefine(std::string DefineName, json11::Json Properties)
{
	Defined.insert_or_assign(DefineName, Properties);
}

json11::Json FFReflectionManager::GetDefined(std::string DefineName)
{
	if (auto it = Defined.find(DefineName); it != Defined.end())
		return it->second;
	else
		return json11::Json();
}

void FFReflectionManager::ExecuteJson(json11::Json JsonData)
{
	if (JsonData == json11::Json())
		debug << "Invalid Json Data";
	auto JsonObject = JsonData.object_items();
	for (int Priority = 0;; Priority++)
	{

	}
	for (auto & JsonIt : JsonObject)
	{
		if (JsonIt.second.is_array())
		{
			for (auto ArgsIt = JsonIt.second.array_items().begin(); ArgsIt < JsonIt.second.array_items().end(); ++ArgsIt)
			{
				if (!ArgsIt->is_object() || !ExecuteOperation(JsonIt.first, ArgsIt->object_items()))
				{
					debug << "Json Execute Error While Command is \"" << JsonIt.first << "\"";
				}
			}
		}
		else
			if (!JsonIt.second.is_array() || !ExecuteOperation(JsonIt.first, JsonIt.second))
			{
				debug << "Json Execute Error While Command is \"" << JsonIt.first << "\"";
			}
	}
}

void FFReflectionManager::ExecuteJsonFile(std::string JsonPath)
{
	std::string JsonParseError;
	auto JsonData = json11::Json::parse(FFData::GetFileDataAsString(JsonPath), JsonParseError);
	if (JsonData == json11::Json())
	{
		debug << "Json Parse Error while Load \"" << JsonPath << "\"";
		return;
	}
	ExecuteJson(JsonData);
}

bool FFReflectionManager::ExecuteOperation(std::string OperationName, const json11::Json& Args, bool* OperationReturn /*= NULL*/)
{
	for (auto & it : ModuleMap)
	{
		auto TmpFunc = (bool(*)(const json11::Json& Args))GetProcAddress(it.second, MAKE_OPERATION_VAR(OperationName).c_str());
		if (TmpFunc != nullptr)
		{
			if (OperationReturn)
			{
				*OperationReturn = TmpFunc(Args);
				if (!*OperationReturn)
					debug << "False is Returned By Operation \"" << OperationName << "\"";
			}
			else
				TmpFunc(Args);
			return true;
		}
	}
	return false;
}

FFReflectionManager* FFReflectionManager::ReflectManager=NULL;