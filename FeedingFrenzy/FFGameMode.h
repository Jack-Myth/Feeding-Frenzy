#pragma once
#include "FFControllers.h"

class FFGameMode //Default Game Mode
{
	FFController* pPlayerController;
protected:
	virtual FFController* CreatePlayerController();
public:
};