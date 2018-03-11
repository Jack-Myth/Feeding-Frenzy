#include "FFUI.h"
#include "FFDelegate.h"
#include "FFReflectionManager.h"

FFUI::FFUI()
{
	SetZOrder(1000);
}

bool FFUI::IsUI()
{
	return true;
}

void FFUI::Render()
{

}

void FFUI::AddChild(FFUI* ChildUI)
{
	Childs.push_back(ChildUI);
}

void FFButtonUI::Tick(float DetailTime)
{

}

REFLECTED_FFACTOR(FFButtonUI, ButtonUI)