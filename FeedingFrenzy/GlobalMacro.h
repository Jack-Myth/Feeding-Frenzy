#pragma once
#pragma warning(disable:4003)

#define GC_INTERVAL 30.f
#define SIMPLE_PROPERTY(TYPE,PROPERTY_NAME,INIT_VALUE)\
private:TYPE PROPERTY_NAME=INIT_VALUE;\
public: void Set##PROPERTY_NAME(TYPE New##PROPERTY_NAME)\
{PROPERTY_NAME=New##PROPERTY_NAME;}\
TYPE Get##PROPERTY_NAME()\
{return PROPERTY_NAME;}private:
#define _MACRO_SPLICE(A,B,C,D) A##B##C##D
#define  MACRO_SPLICE(A,B,C,D) _MACRO_SPLICE(A,B,C,D)
#define OPERATION_PREFIX __OPERATION_
#define OPERATION_PREFIX_STR "__OPERATION_"
#define MAKE_OPERATION_VAR(OPERATION_NAME) (std::string(OPERATION_PREFIX_STR)+OPERATION_NAME)
#define DECLARE_OPERATION(OPERATION_NAME) \
extern "C" __declspec(dllexport)\
bool MACRO_SPLICE(OPERATION_PREFIX,OPERATION_NAME)(const json11::Json& Args)

#define REFLECTED_FFACTOR(CLASS_NAME,REFLECT_NAME)\
std::string CLASS_NAME::Type=FFReflectionManager::Get().__ToReflect(#REFLECT_NAME, FFDelegate<FFActor*(json11::Json::object)>::CreateFromStatic(&CLASS_NAME::__ReflectConstructor)); 

#define REFLECT_ACTOR \
static std::string Type;\
public: \
virtual inline std::string GetType() override\
{\
	return Type;\
}\
static inline FFActor* __ReflectConstructor(json11::Json::object Args)