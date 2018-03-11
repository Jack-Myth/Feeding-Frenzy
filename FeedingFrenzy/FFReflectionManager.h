#pragma once
#include <map>
#include "FFDelegate.h"
#include "json11.hpp"
#include <wtypes.h>
#include "FFDebug.h"

class FFReflectionManager
{
	friend int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd);
	static FFReflectionManager* ReflectManager;
	std::map<std::string, FFDelegate<class FFActor*(json11::Json::object)>> FFActorReflectFunctions;
	std::map<std::string, HMODULE> ModuleMap;
	std::map<std::string, json11::Json> Defined;
public:
	static FFReflectionManager & Get();
	const std::string& __ToReflect(const std::string& RefrectName, const FFDelegate<class FFActor*(json11::Json::object)>& ReflectDelegate);
	FFDelegate<class FFActor*(json11::Json::object)> GetActorTypeConstructor(const std::string& ReflectName);
	std::map<std::string, HMODULE> GetModules();
	bool LoadLibrary(std::string LibraryPath);
	bool UnloadLibrary(std::string LibraryPath);
	void SetDefine(std::string DefineName, json11::Json Properties);
	json11::Json GetDefined(std::string DefineName);
	void ExecuteJson(json11::Json JsonData);
	void ExecuteJsonFile(std::string JsonPath);
	bool ExecuteOperation(std::string OperationName, const json11::Json& Args, bool* OperationReturn = NULL);
};