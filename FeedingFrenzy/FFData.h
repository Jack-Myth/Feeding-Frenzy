#pragma once
#include <map>
#include <string>
#include "json11.hpp"
#include <stack>

class FFData
{
	static std::map<std::string, double> NumbericData;
	static std::map<std::string, std::string> StringData;
	static std::map<std::string, bool> BoolData;
public:
	static std::string GetFileDataAsString(std::string FileName);
	void JsonPraseData(std::string JsonFilePath); //Parse Json Data
	static bool GetBoolData(const std::string Key, const bool DefValue);
	static double GetNumbericData(const std::string Key, const double DefValue);
	static std::string GetStringData(const std::string Key, const std::string DefValue);
};