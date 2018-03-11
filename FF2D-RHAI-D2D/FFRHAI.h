#pragma once

class FFRHAI
{
	//UNDONE:Maybe It will be use to Cross Platform
public:
	static FFRHAI* InitializeRHAI();
	class RHAI_Image
	{
		virtual void* Create() = 0;
		virtual void* CreateFromImage(std::string FileName) = 0;
	};
	virtual void DrawImage(const void*& ImageToDraw) = 0;
};
extern "C" __declspec(dllexport)
static FFRHAI* CreateRAHI();