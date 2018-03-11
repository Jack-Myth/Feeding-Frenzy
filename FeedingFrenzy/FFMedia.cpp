#include "FFMedia.h"
#include <wincodec.h>
#include "WaveFile.h"
#include "FF2DBase.h"
#include "FFHAL.h"

bool FFResource::FFResourceInfo::OnGC()
{
	delete pResource;
	return true;
}

FFResource::FFResource()
{

}

FFResource::FFResourceInfo* FFResource::LoadResource(std::string FilePath)
{
	
}

void FFResource::LoadResourceFrom(std::string FilePath)
{
	if (ResourceInfo->pResource)
		StaticResourceInfoes.find(ResourceInfo->FilePath)->second->RefCount--;
	auto it = StaticResourceInfoes.find(FilePath);
	if (it != StaticResourceInfoes.end())
	{
		ResourceInfo = it->second;
	}
	else
	{
		ResourceInfo = LoadResource(FilePath);
		if (ResourceInfo)
		{
			StaticResourceInfoes.insert_or_assign(FilePath, ResourceInfo);
		}
		else
			return;
	}
}

const FFResource::FFResourceInfo* FFResource::GetResourceInfo() const
{
	return ResourceInfo;
}

void FFResource::ToCopy(const FFResource& Origin)
{
	if (Origin.GetResourceInfo())
	{
		if (ResourceInfo)
		{
			StaticResourceInfoes.find(ResourceInfo->FilePath)->second->RefCount--;
		}
		else
		{
			ResourceInfo = new FFResourceInfo();
			*ResourceInfo = *Origin.GetResourceInfo();
		}
		StaticResourceInfoes.find(Origin.GetResourceInfo()->FilePath)->second->RefCount++;
	}
	else
	{
		if (ResourceInfo)
		{
			StaticResourceInfoes.find(ResourceInfo->FilePath)->second->RefCount--;
			delete ResourceInfo;
			ResourceInfo = nullptr;
		}
	}
}

void FFResource::StartGC()
{
	for (auto it = StaticResourceInfoes.begin(); it != StaticResourceInfoes.end(); ++it)
	{
		if (it->second->RefCount <= 0)
		{
			if (it->second->OnGC())
			{
				delete it->second;
				it = StaticResourceInfoes.erase(it);
			}
		}
	}
}

std::map<std::string, FFResource::FFResourceInfo*> FFResource::StaticResourceInfoes;

FFBitmap::FFBitmap(std::string FilePath) :FFResource(FilePath)
{

}

FFBitmap::FFBitmap()
{

}

ID2D1Bitmap * FFBitmap::GetBitmap()
{
	return (ID2D1Bitmap*)(ResourceInfo->pResource);
}

FFResource::FFResourceInfo* FFBitmap::LoadResource(std::string FilePath)
{
	FFResourceInfo* tmpFFResourceInfo = new FFResourceInfo();
	tmpFFResourceInfo->FilePath = FilePath;
	tmpFFResourceInfo->RefCount = 1;
	if(FFHAL::GetRenderHAI()->ImageUtils->LoadImage(&tmpFFResourceInfo->pResource, FilePath))
		return tmpFFResourceInfo;
	else
	{
		delete tmpFFResourceInfo;
		return nullptr;
	}

	/*WCHAR* ImagePath = new WCHAR[FilePath.length()];
	mbstowcs_s(nullptr, ImagePath, FilePath.length(), FilePath.c_str(), FilePath.length());
	ID2D1Bitmap* tmpBitmapRef;
	IWICBitmap* m_pWicBitmap = NULL;
	IWICBitmapDecoder* m_pWicDecoder = NULL;
	IWICBitmapFrameDecode* m_pWicFrameDecoder = NULL;
	if (FAILED(Sys.WICImagingFactory->CreateDecoderFromFilename(ImagePath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_pWicDecoder)))
		return NULL;
	m_pWicDecoder->GetFrame(0, &m_pWicFrameDecoder);
	IWICBitmapSource* pWicSource = NULL;
	m_pWicFrameDecoder->QueryInterface(IID_PPV_ARGS(&pWicSource));
	IWICFormatConverter* pCovert = NULL;
	Sys.WICImagingFactory->CreateFormatConverter(&pCovert);
	pCovert->Initialize(
		pWicSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeCustom
	);
	Sys.WICImagingFactory->CreateBitmapFromSource(pCovert, WICBitmapCacheOnDemand, &m_pWicBitmap);
	Sys.D2D1HwndRenderTarget->CreateBitmapFromWicBitmap(m_pWicBitmap, NULL, &tmpBitmapRef);
	pCovert->Release();
	m_pWicDecoder->Release();
	m_pWicFrameDecoder->Release();
	FFResourceInfo* tmpReousrceInfo = new FFResourceInfo;
	tmpReousrceInfo->FilePath = FilePath;
	tmpReousrceInfo->pResource = tmpBitmapRef;
	tmpReousrceInfo->RefCount = 1;
	return tmpReousrceInfo;*/
}

void FFBitmap::Draw(const FFRect& DestRect, float opacity, const FFRect& SourceRect)
{
	//TODO:It Need To Be Modify To Fit FFRHAI
	if (!ResourceInfo||!ResourceInfo->pResource)
		return;
	FFHAL::GetRenderHAI()->DrawImage(ResourceInfo->pResource, SourceRect, DestRect, opacity);
}

FFBitmap & FFBitmap::operator=(const FFBitmap& NewBitmap)
{
	FFResource::ToCopy(NewBitmap);
	return *this;
}

FFResource::FFResourceInfo* FFAudio::LoadResource(std::string FilePath)
{
	FFResourceInfo *tmpResourceInfo= new FFResourceInfo();
	tmpResourceInfo->FilePath = FilePath;
	tmpResourceInfo->RefCount = 1;
	if (FFHAL::GetAudioHAI()->LoadSound(&tmpResourceInfo->pResource, FilePath))
	{
		return tmpResourceInfo;
	}
	delete tmpResourceInfo;
	return nullptr;
}

FFAudio::FFAudio(std::string FilePath) :FFResource(FilePath)
{

}

void FFAudio::Play()
{
	if (ResourceInfo&&ResourceInfo->pResource)
	{
	}
}

void FFAudio::PlayAtLocation()
{

}

void FFAudio::Stop()
{
	if (XAudio2SourceVoice)
	{
		XAudio2SourceVoice->Stop();
	}
}

FFAudio & FFAudio::operator=(const FFAudio& NewAudio)
{
	FFResource::ToCopy(NewAudio);
	return *this;
}
