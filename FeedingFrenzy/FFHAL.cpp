#include "FFHAL.h"
#include "FF2DBase.h"

FFRHAI* FFHAL::RenderHAI;

FFAHAI* FFHAL::AudioHAI;

bool FFHAL::InitHAL()
{
	if (!RenderHAI)
	{
		std::string RHAIFileName= "RAHI-D2D.DLL";
		if (FFCmd::IsCmdContain("-D2D"))
		{
			RHAIFileName = "RAHI-D2D.DLL";
		}
		else if (FFCmd::IsCmdContain("-GDI"))
		{
			RHAIFileName = "RAHI-GDI.DLL";
		}
		auto RHAIModule= LoadLibraryA(RHAIFileName.c_str());
		auto RHAIConstructor= (FFRHAI*(*)())GetProcAddress(RHAIModule, "CreateRAHI");
		if (!RHAIConstructor)
			return false;
		RenderHAI = RHAIConstructor();
		return true;
	}
	return false;
}

FFRHAI* FFHAL::GetRenderHAI()
{
	return RenderHAI;
}

FFAHAI* FFHAL::GetAudioHAI()
{
	return AudioHAI;
}
