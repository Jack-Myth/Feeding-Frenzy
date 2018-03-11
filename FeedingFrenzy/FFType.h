#pragma once
#include <utility>
#include <cmath>

struct FFVector2D
{
	float X;
	float Y;
	FFVector2D();
	//FFVector2D(const D2D1_POINT_2F& D2DPoint);
	FFVector2D(const float& X, const float& Y);
	FFVector2D(const float& Size);
	FFVector2D&& operator-();
	FFVector2D&& operator-(const FFVector2D& Target);
	FFVector2D& operator-=(const FFVector2D& Target);
	FFVector2D&& operator+(const FFVector2D& Target);
	FFVector2D& operator+=(const FFVector2D& Target);
	FFVector2D&& operator*(const FFVector2D& Target);
	FFVector2D& operator*=(const FFVector2D& Target);

	//D2D1_POINT_2F ToD2D1Point();
	float GetLength();
	FFVector2D Normalize();
};

struct FFRect
{
	float Left = 0;
	float Top = 0;
	float Right = 0;
	float Buttom = 0;
public:
	FFRect()=default;
	FFRect(const FFVector2D& LeftTop, const FFVector2D& RightButtom);
	FFRect(float left, float top, float right, float buttom);
	float GetWidth();
	float GetHeight();
	FFRect&& operator*(float Multip);
};

struct FFTransform  //2D Transform
{
	FFVector2D Location;
	float Rot = 0.f;
	FFVector2D Scale;
	FFTransform(const FFVector2D& Location = FFVector2D(), const FFVector2D& Scale = FFVector2D(1));
	FFTransform WorldToScreen();
	FFTransform ScreenToWorld();
};

struct FFColor
{
	using byte = unsigned char;
	byte R;
	byte G;
	byte B;
	byte A;
	FFColor(byte R=0, byte G=0, byte B=0, byte A=255);
};