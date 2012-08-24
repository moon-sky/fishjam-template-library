/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


////////////////////////////////////////////////////////////
//
// Implementation of plugin entry points (NPP_*)
// most are just empty stubs for this particular plugin 
//

#include "stdafx.h"
#include "plugin.h"

char*
NPP_GetMIMEDescription(void)
{
	FUNCTION_BLOCK_TRACE(0);
	return "application/npapidemo|application/x-newtype";
}



NPError NPP_Initialize(void)
{
	FUNCTION_BLOCK_TRACE(0);
	return NPERR_NO_ERROR;
}

void NPP_Shutdown(void)
{
	FUNCTION_BLOCK_TRACE(0);
}

// here the plugin creates an instance of our CPlugin object which 
// will be associated with this newly created plugin instance and 
// will do all the neccessary job
NPError NPP_New(NPMIMEType pluginType,
				NPP instance,
				uint16_t mode,
				int16_t argc,
				char* argn[],
				char* argv[],
				NPSavedData* saved)
{   
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;

	CPlugin * pPlugin = new CPlugin(instance);
	if(pPlugin == NULL)
	{
		return NPERR_OUT_OF_MEMORY_ERROR;
	}

	instance->pdata = (void *)pPlugin;
	return rv;
}

// here is the place to clean up and destroy the CPlugin object
NPError NPP_Destroy (NPP instance, NPSavedData** save)
{
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;

	CPlugin * pPlugin = (CPlugin *)instance->pdata;
	if(pPlugin != NULL) {
		pPlugin->shut();
		delete pPlugin;
	}
	return rv;
}

// during this call we know when the plugin window is ready or
// is about to be destroyed so we can do some gui specific
// initialization and shutdown
NPError NPP_SetWindow (NPP instance, NPWindow* pNPWindow)
{    
	if(instance == NULL)
	{	
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;

	if(pNPWindow == NULL)
	{
		return NPERR_GENERIC_ERROR;
	}

	CPlugin * pPlugin = (CPlugin *)instance->pdata;

	if(pPlugin == NULL) 
	{
		return NPERR_GENERIC_ERROR;
	}
	
	// window just created
	if(!pPlugin->isInitialized() && (pNPWindow->window != NULL)) { 
		if(!pPlugin->init(pNPWindow)) {
		delete pPlugin;
		pPlugin = NULL;
		return NPERR_MODULE_LOAD_FAILED_ERROR;
		}
	}
	
	// window goes away
	if((pNPWindow->window == NULL) && pPlugin->isInitialized())
	{
		return NPERR_NO_ERROR;
	}

	// window resized
	if(pPlugin->isInitialized() && (pNPWindow->window != NULL))
	{
		return NPERR_NO_ERROR;
	}

	// this should not happen, nothing to do
	if((pNPWindow->window == NULL) && !pPlugin->isInitialized())
	{
		return NPERR_NO_ERROR;
	}

	return rv;
}

// ==============================
// ! Scriptability related code !
// ==============================
//
// here the plugin is asked by Mozilla to tell if it is scriptable
// we should return a valid interface id and a pointer to 
// nsScriptablePeer interface which we should have implemented
// and which should be defined in the corressponding *.xpt file
// in the bin/components folder
NPError	NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
	FTLTRACE(TEXT("NPP_GetValue want %s\n"), CFnpAPIUtil::GetNPPVariableString(variable));

	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;

	if(instance == NULL)
	{
		return NPERR_GENERIC_ERROR;
	}

	CPlugin * plugin = (CPlugin *)instance->pdata;
	if(plugin == NULL)
	{
		return NPERR_GENERIC_ERROR;
	}

	switch (variable) {
		case NPPVpluginNameString:
		  *((char **)value) = "npAPIDemo";
		  break;
		case NPPVpluginDescriptionString:
		  *((char **)value) = "npAPI Plugin Demo";
		  break;
		case NPPVpluginScriptableNPObject:
		  *(NPObject **)value = plugin->GetScriptableObject();
		  break;
		default:
		  rv = NPERR_GENERIC_ERROR;
	}

	return rv;
}

NPError NPP_NewStream(NPP instance,
					  NPMIMEType type,
					  NPStream* stream, 
					  NPBool seekable,
					  uint16_t* stype)
{
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;
	return rv;
}

int32_t NPP_WriteReady (NPP instance, NPStream *stream)
{
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	int32_t rv = 0x0fffffff;
	return rv;
}

int32_t NPP_Write (NPP instance, NPStream *stream, int32_t offset, int32_t len, void *buffer)
{   
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	int32_t rv = len;
	return rv;
}

NPError NPP_DestroyStream (NPP instance, NPStream *stream, NPError reason)
{
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;
	return rv;
}

void NPP_StreamAsFile (NPP instance, NPStream* stream, const char* fname)
{
	//CFConversion().UTF8_TO_TCHAR(fname);
	FTLTRACE(TEXT("%s, fname=%s\n"), TEXT(__FUNCTION__), CA2T(fname));
	if(instance == NULL)
	{
		return;
	}
}

void NPP_Print (NPP instance, NPPrint* printInfo)
{
	if(instance == NULL)
	{
		return;
	}
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
	if(instance == NULL)
	{
		return;
	}
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
	if(instance == NULL)
	{
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	NPError rv = NPERR_NO_ERROR;
	return rv;
}

int16_t NPP_HandleEvent(NPP instance, void* event)
{
	if(instance == NULL)
	{
		return 0;
	}

	int16_t rv = 0;
	CPlugin * pPlugin = (CPlugin *)instance->pdata;
	if (pPlugin)
	{
		rv = pPlugin->handleEvent(event);
	}

	return rv;
}

NPObject *NPP_GetScriptableInstance(NPP instance)
{
	if(!instance)
	{
		return 0;
	}

	NPObject *npobj = 0;
	CPlugin * pPlugin = (CPlugin *)instance->pdata;
	if (!pPlugin)
	{
		npobj = pPlugin->GetScriptableObject();
	}

	return npobj;
}
