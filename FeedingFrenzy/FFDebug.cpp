#include "FFDebug.h"

FFDebug::FFDebugOutputSection::~FFDebugOutputSection()
{
	std::cout << std::endl;
}

FFDebug::FFDebugOutputSection && FFDebug::FFDebugOutputSection::operator<<(std::string OuputInfomation)
{
	std::cout << OuputInfomation;
	return std::move(*this);
}

FFDebug::FFDebugOutputSection && FFDebug::FFDebugOutputSection::operator<<(const char*& OuputInfomation)
{
	std::cout << OuputInfomation;
	return std::move(*this);
}

FFDebug debug;