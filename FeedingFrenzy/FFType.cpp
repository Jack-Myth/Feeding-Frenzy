#include "FFType.h"

FFVector2D::FFVector2D()
{
	X = Y = 0;
}

//FFVector2D::FFVector2D(const D2D1_POINT_2F& D2DPoint)
//{
//	X = D2DPoint.x;
//	Y = D2DPoint.y;
//}

FFVector2D::FFVector2D(const float& X, const float& Y)
{
	this->X = X;
	this->Y = Y;
}

FFVector2D::FFVector2D(const float& Size)
{
	X = Y = Size;
}

FFVector2D& FFVector2D::operator-=(const FFVector2D& Target)
{
	X -= Target.X;
	Y -= Target.X;
	return *this;
}

FFVector2D&& FFVector2D::operator-(const FFVector2D& Target)
{
	FFVector2D tmpVector2;
	tmpVector2.X = X - Target.X;
	tmpVector2.Y = Y - Target.Y;
	return std::move(tmpVector2);
}

FFVector2D&& FFVector2D::operator-()
{
	FFVector2D tmpVector2;
	tmpVector2.X = -X;
	tmpVector2.Y = -Y;
	return std::move(tmpVector2);
}

FFVector2D& FFVector2D::operator+=(const FFVector2D& Target)
{
	X += Target.X;
	Y += Target.Y;
	return *this;
}

FFVector2D&& FFVector2D::operator+(const FFVector2D& Target)
{
	FFVector2D tmpVector2;
	tmpVector2.X = X + Target.X;
	tmpVector2.Y = Y + Target.Y;
	return std::move(tmpVector2);
}

FFVector2D& FFVector2D::operator*=(const FFVector2D& Target)
{
	X *= Target.X;
	Y *= Target.Y;
	return *this;
}

FFVector2D&& FFVector2D::operator*(const FFVector2D& Target)
{
	FFVector2D tmpVector2;
	tmpVector2.X = X * Target.X;
	tmpVector2.Y = Y * Target.Y;
	return std::move(tmpVector2);
}

//D2D1_POINT_2F FFVector2D::ToD2D1Point()
//{
//	return D2D1::Point2F(X, Y);
//}

float FFVector2D::GetLength()
{
	return std::sqrt(X*X + Y * Y);
}

FFVector2D FFVector2D::Normalize()
{
	FFVector2D tmpVector2;
	float Norm = GetLength();
	tmpVector2.X = X / Norm;
	tmpVector2.Y = Y / Norm;
	return std::move(tmpVector2);
}

FFTransform::FFTransform(const FFVector2D& Location /*= FFVector2D()*/, const FFVector2D& Scale /*= FFVector2D(1)*/)
{
	this->Location = Location;
	this->Scale = Scale;
}

//TODO:Implement WTS And STW Function, This is important
FFTransform FFTransform::WorldToScreen()
{
	return FFTransform();
	
}

FFTransform FFTransform::ScreenToWorld()
{
	return FFTransform();
}

FFRect::FFRect(const FFVector2D& LeftTop, const FFVector2D& RightButtom)
{
	Left = LeftTop.X;
	Top = LeftTop.Y;
	Right = RightButtom.X;
	Buttom = RightButtom.Y;
}

FFRect::FFRect(float left, float top, float right, float buttom)
{
	Left = left;
	Top = top;
	Right = right;
	Buttom = buttom;
}

float FFRect::GetWidth()
{
	return Right - Left;
}

float FFRect::GetHeight()
{
	return Buttom - Top;
}

FFRect&& FFRect::operator*(float Multip)
{
	FFVector2D HalfPosition((Right - Left) / 2 + Left, (Buttom - Top) / 2 + Top);
	FFRect tmpRect;
	tmpRect.Right = (Right - HalfPosition.X)*Multip+ HalfPosition.X;
	tmpRect.Left = (HalfPosition.X - Left)*Multip+ HalfPosition.X;
	tmpRect.Buttom = (Buttom - HalfPosition.Y)*Multip+ HalfPosition.Y;
	tmpRect.Top = (HalfPosition.Y - Buttom)*Multip + HalfPosition.Y;
	return std::move(tmpRect);
}

FFColor::FFColor(byte R/*=0*/, byte G/*=0*/, byte B/*=0*/, byte A/*=255*/)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}
