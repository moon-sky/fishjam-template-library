/**
* @file ScriptablePluginObjectBase.cpp
* @brief NPAPI use class implementation file
* @author : Woo Chang-Hee(changhee.woo@nhn.com)
* @date : 2011-06-20
* Copyright (c) 2011 NHN / NBP Corporation. All Rights Reserved.
*/
#include "stdafx.h"
#include "ScriptablePluginObjectBase.h"

void
ScriptablePluginObjectBase::Invalidate()
{
}

bool
ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
								   uint32_t argCount, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
										  uint32_t argCount, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
	FTLASSERT(FALSE);
	std::string strName = NPN_UTF8FromIdentifier(name);
	FTLTRACE(TEXT("%s, name=%s\n"), TEXT(__FUNCTION__), CFConversion().UTF8_TO_TCHAR(strName.c_str()));
	return false;
}

bool 
ScriptablePluginObjectBase::Deallocate()
{
	FTLASSERT(FALSE);
	return false;
}

bool
ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
	FTLASSERT(FALSE);
	return false;
}

bool
ScriptablePluginObjectBase::SetProperty(NPIdentifier name,
										const NPVariant *value)
{
	FTLASSERT(FALSE);
	return false;
}

bool
ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
	FTLASSERT(FALSE);
	return false;
}

bool
ScriptablePluginObjectBase::Enumerate(NPIdentifier **identifier,
									  uint32_t *count)
{
	return false;
}

bool
ScriptablePluginObjectBase::Construct(const NPVariant *args, uint32_t argCount,
									  NPVariant *result)
{
	return false;
}

// static
void
ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
	// Call the virtual destructor.
	((ScriptablePluginObjectBase *)npobj)->Deallocate();
	delete (ScriptablePluginObjectBase *)npobj;
}

// static
void
ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
	((ScriptablePluginObjectBase *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
									const NPVariant *args, uint32_t argCount,
									NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->Invoke(name, args, argCount, result);
}

// static
bool
ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
										   const NPVariant *args,
										   uint32_t argCount,
										   NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->InvokeDefault(args, argCount,
		result);
}

// static
bool
ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
										 NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
										 const NPVariant *value)
{
	return ((ScriptablePluginObjectBase *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->RemoveProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_Enumerate(NPObject *npobj,
									   NPIdentifier **identifier,
									   uint32_t *count)
{
	return ((ScriptablePluginObjectBase *)npobj)->Enumerate(identifier, count);
}

// static
bool
ScriptablePluginObjectBase::_Construct(NPObject *npobj, const NPVariant *args,
									   uint32_t argCount, NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->Construct(args, argCount,
		result);
}
