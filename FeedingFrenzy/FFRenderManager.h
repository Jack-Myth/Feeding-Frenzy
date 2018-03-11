#pragma once
#include "FFDelegate.h"

class FFRenderManager
{
	static FFRenderManager* pRenderManager;
	FFRenderManager();
public:
	struct FFRenderInfo
	{
		FFDelegate<void()>* RenderDelegate = NULL;
		int* pZOrder;
		FFRenderInfo(const FFDelegate<void()>& tmplateRenderDelegate, int* pZOrder);
		~FFRenderInfo();
	};
private:
	std::vector<FFRenderInfo*> RenderNotify;
public:
	static FFRenderManager & Get();
	void AddRenderDelegate(FFRenderInfo* pRenderInfo);
	bool RemoveRenderDelegate(FFRenderInfo* pRender);
	void NotifyRender();
};
typedef FFRenderManager::FFRenderInfo FFRenderInfo;