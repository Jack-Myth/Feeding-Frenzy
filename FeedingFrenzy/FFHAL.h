#pragma once
#include "FFType.h"
#include <string>

//FF2D Render Hardware Abstract Interface
class FFRHAI
{
	//UNDONE:Maybe It will be use to Cross Platform
public:
	struct TextStyle
	{
		enum class Gravity
		{
			G_Left,
			G_Right,
			G_Center
		};
		std::string FontName="Î¢ÈíÑÅºÚ";
		float FontSize=12.f;
		FFColor FontColor;
		bool IsItalic=false;
		bool IsBold = false;
		Gravity TextGravity = Gravity::G_Left;
	};
	class RHAI_Image
	{
	public:
		//Create a Empty Image
		virtual void CreateEmpty(void** pBitmap) =0;

		//Fill Image With Given Color
		//Image Must Exist ,or Crush May Occur
		virtual bool FillOfColor(void** pBitmap,const FFColor& ColorToFill)=0;

		//Load Image From Desk File
		//False is returned if Load Failed(like File is not accessible)
		virtual bool LoadImage(void** pBitmap, std::string FileName)=0;

		//Destroy the Given Image And Release the Resources
		//Remember Set pointer To nullptr
		//Do not Destroy Multiple Times
		virtual void Destroy(void* pBitmap) = 0;

		//Return Size Of the Given pBitmap
		//Size's Unit is Pixel
		virtual FFRect* GetSize(void* pBitmap) = 0;
	}*ImageUtils;
	//Draw Image To the Target Position of the Viewport
	virtual void DrawImage(void*const& ImageToDraw,const FFRect& SourceRect,const FFRect& TargetRect,float Opacity=1.f) = 0;

	//one Overload of DrawImage,Draw All part of SourceImage
	//Equal to DrawImage(ImageToDraw,FFRect(0.f,0.f1.f,1.f),TargetRect,OPaci)
	virtual void DrawImage(void*const& ImageToDraw, const FFRect& TargetRect, float Opacity = 1.f) = 0;

	//Draw a Rect To Viewport
	virtual void DrawRect(const FFRect& Target, const FFColor& ColorToFill, bool isHollow=false,float EdgeLength=0) = 0;

	//Draw a Circle To Viewport
	virtual void DrawCircle(FFVector2D CircleCenter, float Radius, bool isHollow, float EdgeLength = 0) = 0;

	//Simple Draw a Text To Viewport
	virtual void DrawText(std::string TextToDraw, std::string FontName, float FontSize, const FFColor& FontColor) = 0;

	//Draw a Text To Viewport
	//This Overload Certain lot's of Settings
	virtual void DrawText(std::string TextToDraw, const TextStyle& Styles) = 0;
};

//FF2D Audio Hardware Abstruct Interface
//TODO:This May Useful, But Put it the last To Implement
class FFAHAI
{
public:
	//Load a Sound from a WAV File
	//if Succeed,return True;
	virtual bool LoadSound(void** pSound, std::string FilePath) = 0;

	//Play Sound At Global,No Position Considered, Always Max Volume
	virtual void PlaySoundGlobal(void* pSound) = 0;

	//Play Sound At Position, Volume Will Change Depend on the position
	virtual void PlaySoundAtPosition(void* pSound,FFVector2D Position) = 0;

	//Destroy the Sound Object and Release all Resources
	//Do not Destroy Multiple Times
	virtual void Destroy(void* pSound)
};

//FF2D Hardware Abstract Layer
class FFHAL
{
	static FFRHAI* RenderHAI;
	static FFAHAI* AudioHAI;
public:
	static bool InitHAL();
	static FFRHAI* GetRenderHAI();
	static FFAHAI* GetAudioHAI();
};