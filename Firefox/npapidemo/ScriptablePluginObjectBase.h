#ifndef __SCRIPTABLE_PLUGIN_OBJECTBASE_INC_
#define __SCRIPTABLE_PLUGIN_OBJECTBASE_INC_

#include <Windows.h>
#include "npapi.h"
#include "npfunctions.h"
#include "npruntime.h"



#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)							\
	static NPClass s##_class##_NPClass = {                                      \
	NP_CLASS_STRUCT_VERSION_CTOR,                                               \
	ctor,                                                                       \
	ScriptablePluginObjectBase::_Deallocate,                                    \
	ScriptablePluginObjectBase::_Invalidate,                                    \
	ScriptablePluginObjectBase::_HasMethod,                                     \
	ScriptablePluginObjectBase::_Invoke,                                        \
	ScriptablePluginObjectBase::_InvokeDefault,                                 \
	ScriptablePluginObjectBase::_HasProperty,                                   \
	ScriptablePluginObjectBase::_GetProperty,                                   \
	ScriptablePluginObjectBase::_SetProperty,                                   \
	ScriptablePluginObjectBase::_RemoveProperty,                                \
	ScriptablePluginObjectBase::_Enumerate,                                     \
	ScriptablePluginObjectBase::_Construct                                      \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass



#define INIT_EXPORT_METHOD_MAP()	_InitMethodRegister();

#define DECLARE_EXPORT_METHOD_MAP(class)	\
public:\
	typedef bool(class::*DISP_METHOD)(const NPVariant *args, uint32_t argCount, NPVariant *result);	\
	typedef std::string		DISP_KEY;	\
	typedef std::map<DISP_KEY, DISP_METHOD>	METHOD_MAP;	\
	typedef METHOD_MAP::iterator			METHOD_ITERATOR;\
	bool _Disp_Register(DISP_KEY key, DISP_METHOD pfn);\
	void _Disp_Unregister(DISP_KEY key);\
	bool _Dispatch(DISP_KEY key, const NPVariant *args, uint32_t argCount, NPVariant *result);\
	bool _IsRegistedMethod(DISP_KEY key);\
	void _InitMethodRegister();	\
	METHOD_MAP		m_objMethod;

#define BEGIN_EXPORT_METHOD_MAP(class)	\
	bool class::_Disp_Register(DISP_KEY key, DISP_METHOD pfn)\
{\
	METHOD_ITERATOR itr;\
	itr = m_objMethod.find(key);\
	if(itr != m_objMethod.end())\
	{\
	return false;\
	}\
	m_objMethod[key] = pfn;\
	return true;\
}\
	void class::_Disp_Unregister(DISP_KEY key)\
{\
	m_objMethod.erase(key);\
}\
	bool class::_Dispatch(DISP_KEY key, const NPVariant *args, uint32_t argCount, NPVariant *result)\
{\
	METHOD_ITERATOR itr = m_objMethod.find(key);\
	if(itr == m_objMethod.end())\
	{\
	return false;\
	}\
	DISP_METHOD pfn = itr->second;\
	return (this->*pfn)(args, argCount, result);\
}\
	bool class::_IsRegistedMethod(DISP_KEY key)\
{\
	METHOD_ITERATOR itr = m_objMethod.find(key);\
	if(itr == m_objMethod.end())\
	{\
	return false;\
	}\
	return true;\
}\
	void class::_InitMethodRegister()\
{

#define REGISTER_EXPORT_METHOD(name, pfn)\
	_Disp_Register(name, pfn);

#define END_EXPORT_METHOD_MAP()\
}

#define INVOKE_EXPORT_METHOD(func_name, args, argCount, result)\
	_Dispatch(func_name, args, argCount, result);

#define TEST_EXPORT_METHOD(func_name)\
	_IsRegistedMethod(func_name);


#define DECLARE_EXPORT_METHOD(name)\
	bool name(const NPVariant *args, uint32_t argCount, NPVariant *result);

/**
* Helper class that can be used to map calls to the NPObject hooks
* into virtual methods on instances of classes that derive from this
* class.
*/
class ScriptablePluginObjectBase : public NPObject
{
public:
	ScriptablePluginObjectBase(NPP npp)
		: mNpp(npp)
	{
	}

	virtual ~ScriptablePluginObjectBase()
	{
	}

	// Virtual NPObject hooks called through this base class. Override
	// as you see fit.
	virtual void Invalidate();
	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool RemoveProperty(NPIdentifier name);
	virtual bool Enumerate(NPIdentifier **identifier, uint32_t *count);
	virtual bool Construct(const NPVariant *args, uint32_t argCount, NPVariant *result);
	virtual bool Deallocate();

public:
	static void _Deallocate(NPObject *npobj);
	static void _Invalidate(NPObject *npobj);
	static bool _HasMethod(NPObject *npobj, NPIdentifier name);
	static bool _Invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool _InvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool _HasProperty(NPObject * npobj, NPIdentifier name);
	static bool _GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
	static bool _SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
	static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);
	static bool _Enumerate(NPObject *npobj, NPIdentifier **identifier, uint32_t *count);
	static bool _Construct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

protected:
	NPP mNpp;
};

#endif // __SCRIPTABLE_PLUGIN_OBJECTBASE_INC_