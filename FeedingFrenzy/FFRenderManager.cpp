#include "FFRenderManager.h"
#include <algorithm>

FFRenderManager::FFRenderInfo::FFRenderInfo(const FFDelegate<void()>& tmplateRenderDelegate, int* pZOrder)
{
	RenderDelegate = new FFDelegate<void()>(tmplateRenderDelegate);
	this->pZOrder = pZOrder;
}

FFRenderManager::FFRenderInfo::~FFRenderInfo()
{
	delete RenderDelegate;
}

FFRenderManager::FFRenderManager()
{

}

FFRenderManager & FFRenderManager::Get()
{
	if (!pRenderManager)
	{
		pRenderManager = new FFRenderManager();
	}
	return *pRenderManager;
}

void FFRenderManager::AddRenderDelegate(FFRenderInfo* pRenderInfo)
{
	RenderNotify.push_back(pRenderInfo);
	std::sort(RenderNotify.begin(), RenderNotify.end(), [](const FFRenderInfo* a, const FFRenderInfo* b)-> bool
			  {
				  return a->pZOrder < b->pZOrder;
			  });
}

bool FFRenderManager::RemoveRenderDelegate(FFRenderInfo* pRender)
{
	auto tmpIt = std::find(RenderNotify.begin(), RenderNotify.end(), pRender);
	if (tmpIt != RenderNotify.end())
	{
		RenderNotify.erase(tmpIt);
		return true;
	}
	else
		return false;
}

void FFRenderManager::NotifyRender()
{
	for (auto it = RenderNotify.begin(); it < RenderNotify.end(); ++it)
	{
		(*it)->RenderDelegate->GetStdFunction()();
	}
}

FFRenderManager* FFRenderManager::pRenderManager = NULL;