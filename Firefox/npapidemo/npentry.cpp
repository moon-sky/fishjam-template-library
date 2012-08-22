/**
* @file npentry.cpp
* @brief NPAPI use class implementation file
* @author : Woo Chang-Hee(changhee.woo@nhn.com)
* @date : 2011-06-20
* Copyright (c) 2011 NHN / NBP Corporation. All Rights Reserved.
*/
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


//////////////////////////////////////////////////////////////
//
// Main plugin entry point implementation
//
#include "stdafx.h"
#include "npapi.h"
#include "npfunctions.h"

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPNetscapeFuncs NPNFuncs;

#ifdef XP_WIN
class CGlobalInit
{
public:
	CGlobalInit()
	{
		FUNCTION_BLOCK_INIT();
	}
	~CGlobalInit()
	{
		FUNCTION_BLOCK_UNINIT();
	}
};

static CGlobalInit g_globalInitObject;

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* pFuncs)
{
	FUNCTION_BLOCK_TRACE(0);

	MessageBox(NULL, TEXT("NP_GetEntryPoints"), NULL, MB_OK);

	if(pFuncs == NULL)
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	if(pFuncs->size < sizeof(NPPluginFuncs))
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	pFuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	pFuncs->newp          = NPP_New;
	pFuncs->destroy       = NPP_Destroy;
	pFuncs->setwindow     = NPP_SetWindow;
	pFuncs->newstream     = NPP_NewStream;
	pFuncs->destroystream = NPP_DestroyStream;
	pFuncs->asfile        = NPP_StreamAsFile;
	pFuncs->writeready    = NPP_WriteReady;
	pFuncs->write         = NPP_Write;
	pFuncs->print         = NPP_Print;
	pFuncs->event         = NPP_HandleEvent;
	pFuncs->urlnotify     = NPP_URLNotify;
	pFuncs->getvalue      = NPP_GetValue;
	pFuncs->setvalue      = NPP_SetValue;
	pFuncs->javaClass     = NULL;

	return NPERR_NO_ERROR;
}

#endif /* XP_WIN */

char *NPP_GetMIMEDescription();

const char *
NP_GetMIMEDescription()
{
	FUNCTION_BLOCK_TRACE(0);
	return NPP_GetMIMEDescription();
}

NPError
NP_GetValue(void* future, NPPVariable variable, void *value)
{
	FUNCTION_BLOCK_TRACE(0);
	::MessageBox(NULL, TEXT("NP_GetValue"), CFnpAPIUtil::GetNPPVariableString(variable), MB_OK);
	return NPP_GetValue((NPP_t *)future, variable, value);
}

NPError OSCALL
NP_Initialize(NPNetscapeFuncs* pFuncs
#ifdef XP_UNIX
			  , NPPluginFuncs* pluginFuncs
#endif
			  )
{
	FUNCTION_BLOCK_TRACE(0);
	MessageBox(NULL, TEXT("NP_Initialize"), NULL, MB_OK);

	if(pFuncs == NULL)
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	if(HIBYTE(pFuncs->version) > NP_VERSION_MAJOR)
	{
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
	}

	if(pFuncs->size < sizeof(NPNetscapeFuncs))
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	NPNFuncs.size                    = pFuncs->size;
	NPNFuncs.version                 = pFuncs->version;
	NPNFuncs.geturlnotify            = pFuncs->geturlnotify;
	NPNFuncs.geturl                  = pFuncs->geturl;
	NPNFuncs.posturlnotify           = pFuncs->posturlnotify;
	NPNFuncs.posturl                 = pFuncs->posturl;
	NPNFuncs.requestread             = pFuncs->requestread;
	NPNFuncs.newstream               = pFuncs->newstream;
	NPNFuncs.write                   = pFuncs->write;
	NPNFuncs.destroystream           = pFuncs->destroystream;
	NPNFuncs.status                  = pFuncs->status;
	NPNFuncs.uagent                  = pFuncs->uagent;
	NPNFuncs.memalloc                = pFuncs->memalloc;
	NPNFuncs.memfree                 = pFuncs->memfree;
	NPNFuncs.memflush                = pFuncs->memflush;
	NPNFuncs.reloadplugins           = pFuncs->reloadplugins;
	NPNFuncs.getJavaEnv              = pFuncs->getJavaEnv;
	NPNFuncs.getJavaPeer             = pFuncs->getJavaPeer;
	NPNFuncs.getvalue                = pFuncs->getvalue;
	NPNFuncs.setvalue                = pFuncs->setvalue;
	NPNFuncs.invalidaterect          = pFuncs->invalidaterect;
	NPNFuncs.invalidateregion        = pFuncs->invalidateregion;
	NPNFuncs.forceredraw             = pFuncs->forceredraw;
	NPNFuncs.getstringidentifier     = pFuncs->getstringidentifier;
	NPNFuncs.getstringidentifiers    = pFuncs->getstringidentifiers;
	NPNFuncs.getintidentifier        = pFuncs->getintidentifier;
	NPNFuncs.identifierisstring      = pFuncs->identifierisstring;
	NPNFuncs.utf8fromidentifier      = pFuncs->utf8fromidentifier;
	NPNFuncs.intfromidentifier       = pFuncs->intfromidentifier;
	NPNFuncs.createobject            = pFuncs->createobject;
	NPNFuncs.retainobject            = pFuncs->retainobject;
	NPNFuncs.releaseobject           = pFuncs->releaseobject;
	NPNFuncs.invoke                  = pFuncs->invoke;
	NPNFuncs.invokeDefault           = pFuncs->invokeDefault;
	NPNFuncs.evaluate                = pFuncs->evaluate;
	NPNFuncs.getproperty             = pFuncs->getproperty;
	NPNFuncs.setproperty             = pFuncs->setproperty;
	NPNFuncs.removeproperty          = pFuncs->removeproperty;
	NPNFuncs.hasproperty             = pFuncs->hasproperty;
	NPNFuncs.hasmethod               = pFuncs->hasmethod;
	NPNFuncs.releasevariantvalue     = pFuncs->releasevariantvalue;
	NPNFuncs.setexception            = pFuncs->setexception;
	NPNFuncs.pluginthreadasynccall   = pFuncs->pluginthreadasynccall;
	NPNFuncs.handleevent			 = pFuncs->handleevent;

#ifdef XP_UNIX
	/*
	* Set up the plugin function table that Netscape will use to
	* call us.  Netscape needs to know about our version and size
	* and have a UniversalProcPointer for every function we
	* implement.
	*/
	pluginFuncs->version    = (NP_VERSION_MAJOR << 8) + NP_VERSION_MINOR;
	pluginFuncs->size       = sizeof(NPPluginFuncs);
	pluginFuncs->newp       = NewNPP_NewProc(NPP_New);
	pluginFuncs->destroy    = NewNPP_DestroyProc(NPP_Destroy);
	pluginFuncs->setwindow  = NewNPP_SetWindowProc(NPP_SetWindow);
	pluginFuncs->newstream  = NewNPP_NewStreamProc(NPP_NewStream);
	pluginFuncs->destroystream = NewNPP_DestroyStreamProc(NPP_DestroyStream);
	pluginFuncs->asfile     = NewNPP_StreamAsFileProc(NPP_StreamAsFile);
	pluginFuncs->writeready = NewNPP_WriteReadyProc(NPP_WriteReady);
	pluginFuncs->write      = NewNPP_WriteProc(NPP_Write);
	pluginFuncs->print      = NewNPP_PrintProc(NPP_Print);
	pluginFuncs->urlnotify  = NewNPP_URLNotifyProc(NPP_URLNotify);
	pluginFuncs->event      = NULL;
	pluginFuncs->getvalue   = NewNPP_GetValueProc(NPP_GetValue);
#ifdef OJI
	pluginFuncs->javaClass  = NPP_GetJavaClass();
#endif

	NPP_Initialize();
#endif

	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown()
{
	FUNCTION_BLOCK_TRACE(0);
	//MessageBox(NULL, TEXT("NP_Shutdown"), NULL, MB_OK);
	return NPERR_NO_ERROR;
}
