#pragma once
#include <iostream>
#include <string>

class FFDebug
{
public:
	class FFDebugOutputSection
	{
	public:
		~FFDebugOutputSection();
		FFDebugOutputSection && operator<<(std::string OuputInfomation);
		FFDebugOutputSection && operator<<(const char*& OuputInfomation);
	};
	template<typename T>
	FFDebugOutputSection&& operator <<(T OutputInfomation)
	{
		FFDebugOutputSection* DebugOutputSection = new FFDebugOutputSection();
		return *DebugOutputSection << (OutputInfomation);
	}
};

extern FFDebug debug;