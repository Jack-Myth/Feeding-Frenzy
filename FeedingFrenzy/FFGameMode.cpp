#include "FFGameMode.h"

FFController* FFGameMode::CreatePlayerController()
{
	return (FFController*)new FFPlayerController;
}

