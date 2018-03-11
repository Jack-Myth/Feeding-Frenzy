#pragma once

class FFGCManager
{
	static FFGCManager* GCManager;
	FFGCManager();
public:
	void StartGC();
	static FFGCManager& Get();
};