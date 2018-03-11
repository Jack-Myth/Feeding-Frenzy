#include "FFData.h"

std::map<std::string, double> FFData::NumbericData;
std::map<std::string, std::string> FFData::StringData;
std::map<std::string, bool> FFData::BoolData;

std::string FFData::GetFileDataAsString(std::string FileName)
{
	FILE* pFile;
	fopen_s(&pFile, FileName.c_str(), "rb");
	if (!pFile)
		return std::string();
	fseek(pFile, 0, SEEK_END);
	int FileSize = ftell(pFile);
	char* FileData = new char[FileSize];
	fseek(pFile, 0, SEEK_SET);
	fread_s(FileData, FileSize, FileSize, 1, pFile);
	fclose(pFile);
	return std::string(FileData, FileSize);
}

void FFData::JsonPraseData(std::string JsonFilePath)  //Parse Json Data;
{
	std::string JsonParseError;
	json11::Json JsonData = json11::Json::parse(GetFileDataAsString(JsonFilePath), JsonParseError);
	if (JsonData.is_null())
		return;
	std::stack<json11::Json> JsonCollection;
	JsonCollection.push(JsonData);
	while (!JsonCollection.empty())
	{
		json11::Json tmpJsonData = JsonCollection.top();
		JsonCollection.pop();
		if (tmpJsonData.is_object())
		{
			for (auto it = tmpJsonData.object_items().begin(); it != tmpJsonData.object_items().end(); ++it)
			{
				if (it->second.is_bool())
				{
					BoolData.insert_or_assign(it->first, it->second.bool_value());
				}
				else if (it->second.is_number())
				{
					NumbericData.insert_or_assign(it->first, it->second.number_value());
				}
				else if (it->second.is_string())
				{
					StringData.insert_or_assign(it->first, it->second.string_value());
				}
				else if (it->second.is_array() || it->second.is_object())
				{
					JsonCollection.push(it->second);
				}
			}
		}
		else if (tmpJsonData.is_array())
		{
			for (auto it = tmpJsonData.array_items().begin(); it < tmpJsonData.array_items().end(); ++it)
			{
				if (it->is_object() || it->is_array())
				{
					JsonCollection.push(*it);
				}
			}
		}
	}
}

bool FFData::GetBoolData(const std::string Key, const bool DefValue)
{
	auto tmpIt = BoolData.find(Key);
	if (tmpIt != BoolData.end())
		return tmpIt->second;
	else
		return DefValue;
}

double FFData::GetNumbericData(const std::string Key, const double DefValue)
{
	auto tmpIt = NumbericData.find(Key);
	if (tmpIt != NumbericData.end())
		return tmpIt->second;
	else
		return DefValue;
}

std::string FFData::GetStringData(const std::string Key, const std::string DefValue)
{
	auto tmpIt = StringData.find(Key);
	if (tmpIt != StringData.end())
		return tmpIt->second;
	else
		return DefValue;
}
