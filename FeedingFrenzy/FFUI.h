#pragma once
#include "FFActor.h"
#include <string>
#include "json11.hpp"

class FFUI :public FFActor
{
	FFUI* ParentUI = NULL;
	std::vector<FFUI*> Childs;
protected:
	FFUI();
	virtual bool IsUI() override;
	virtual void Render() override;
	void AddChild(FFUI* ChildUI);
public:
	typedef FFDelegate<bool(FFUI*)> UIMouseEvent;
};


class FFButtonUI :public FFUI
{
	REFLECT_ACTOR
	{
		return new FFButtonUI;
	}
		using FFUI::UIMouseEvent;
	UIMouseEvent OnHovered;
	UIMouseEvent OnLeaved;
	UIMouseEvent OnPressed;
	UIMouseEvent OnReleased;
	UIMouseEvent OnClicked;
public:
	virtual void Tick(float DetailTime) override;
};


class FFImageUI :public FFUI
{
	REFLECT_ACTOR
	{

	}
};

class FFTextViewUI :public FFUI
{

};