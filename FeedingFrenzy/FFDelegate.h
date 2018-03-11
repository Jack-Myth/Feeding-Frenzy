#pragma once
#include <functional>

template<typename T>
class FFDelegate	//Base Delegate Class
{
#define __PH std::placeholders
#define _1PLCAE_HOLDER __PH::_1
#define _2PLCAE_HOLDER __PH::_1,__PH::_2
#define _3PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3
#define _4PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4
#define _5PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4,__PH::_5
#define _6PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4,__PH::_5,__PH::_6
#define _7PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4,__PH::_5,__PH::_6,__PH::_7
#define _8PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4,__PH::_5,__PH::_6,__PH::_7,__PH::_8
#define _9PLCAE_HOLDER __PH::_1,__PH::_2,__PH::_3,__PH::_4,__PH::_5,__PH::_6,__PH::_7,__PH::_8,__PH::_9
	std::function<T> DelegateFunc;
	bool IsFunctionBound = false;
public:
	bool IsBound()
	{
		return IsFunctionBound;
	}
	void BindStatic(std::function<T> StaticFunc)
	{
		DelegateFunc = StaticFunc;
		IsFunctionBound = true;
	}

	void BindLambda(std::function<T> LambdaFunc)
	{
		DelegateFunc = LambdaFunc;
		IsFunctionBound = true;
	}

	template<typename _Func, typename... _Paramters>
	void BindInstane(const _Func& Func, _Paramters&&... Params)
	{
		DelegateFunc = std::bind(Func, Params...);
		IsFunctionBound = true;
	}
	std::function<T>& GetStdFunction()
	{
		return DelegateFunc;
	}

	static auto CreateFromLambda(std::function<T> LambdaFunc) ->FFDelegate<T> const
	{
		FFDelegate<T> tmpDelegate;
		tmpDelegate.BindLambda(LambdaFunc);
		return tmpDelegate;
	}

	static auto CreateFromStatic(std::function<T> StaticFunc) ->FFDelegate<T> const
	{
		FFDelegate<T> tmpDelegate;
		tmpDelegate.BindStatic(StaticFunc);
		return tmpDelegate;
	}

	template<typename _Func, typename... _Paramters>
	static auto CreateFromInstance(const _Func& Func, _Paramters&&... Params) ->FFDelegate<T> const
	{
		FFDelegate<T> tmpDelegate;
		tmpDelegate.BindInstane(Func, Params...);
		return tmpDelegate;
	}
};