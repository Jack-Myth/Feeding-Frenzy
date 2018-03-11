#pragma once
#include <string>
#include <map>
#include <d2d1.h>
#include <xaudio2.h>
#include "FFType.h"

class FFResource
{
protected:
	struct FFResourceInfo
	{
		std::string FilePath;
		void* pResource = NULL;
		int RefCount = 0;
		friend FFResource;
	protected:
		virtual bool OnGC();
	};
private:
	static std::map<std::string, FFResourceInfo*> StaticResourceInfoes;
protected:
	FFResourceInfo * ResourceInfo = NULL;
	virtual FFResourceInfo* LoadResource(std::string FilePath) = 0;
public:
	FFResource();
	FFResource(std::string FilePath);
	void LoadResourceFrom(std::string FilePath);
	const FFResourceInfo* GetResourceInfo() const;
	void ToCopy(const FFResource& Origin);
	static void StartGC();
	virtual ~FFResource()
	{
		StaticResourceInfoes.find(ResourceInfo->FilePath)->second->RefCount--;
	}
};

class FFBitmap :public FFResource
{
public:
	FFBitmap();
	FFBitmap(std::string FilePath);
	ID2D1Bitmap * GetBitmap();
protected:
	virtual FFResourceInfo* LoadResource(std::string FilePath) override;
	void Draw(const FFRect& DestRect, float opacity, const FFRect& SourceRect);
public:
	FFBitmap & operator=(const FFBitmap& NewBitmap);


	inline D2D1_SIZE_U GetSizeU()
	{
		return GetBitmap()->GetPixelSize();
	}
	inline D2D1_SIZE_F GetSizeF()
	{
		D2D1_SIZE_U DestSize = GetBitmap()->GetPixelSize();
		return D2D1::SizeF((FLOAT)DestSize.width, (FLOAT)DestSize.height);
	}
	inline D2D1_RECT_F GetFullSizeRectF()
	{
		D2D1_SIZE_U DestSize = GetBitmap()->GetPixelSize();
		return D2D1::RectF(0, 0, (FLOAT)DestSize.width, (FLOAT)DestSize.height);
	}
	inline D2D1_RECT_U GetFullSizeRectU()
	{
		D2D1_SIZE_U DestSize = GetBitmap()->GetPixelSize();
		return D2D1::RectU(0, 0, DestSize.width, DestSize.height);
	}
};

class FFAudio :public FFResource
{
protected:
	virtual FFResourceInfo* LoadResource(std::string FilePath) override;
public:
	FFAudio(std::string FilePath);
	FFAudio & operator=(const FFAudio& NewAudio);
	WAVEFORMATEX GetWaveFormat();
	void Play();
	void PlayAtLocation();
	void Stop();
};