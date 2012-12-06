#include "stdafx.h"

#include "DebugInfoFilter.h"
#include "DebugInfoFilterUuids.h"
#include "DebugInfoFilterProperty.h"
#include "GraphInfoPropertyPage.h"

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_Video,       // Major type
	&MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN psudPins[] =
{
	{
		L"Input",           // String pin name
			FALSE,              // Is it rendered
			FALSE,              // Is it an output
			FALSE,              // Allowed none
			FALSE,              // Allowed many
			&CLSID_NULL,        // Connects to filter
			NULL,		         // Connects to pin
			1,                  // Number of types
			&sudPinTypes 
	},     // The pin details
	{ 
		L"Output",				// String pin name
			FALSE,              // Is it rendered
			TRUE,               // Is it an output
			FALSE,              // Allowed none
			FALSE,              // Allowed many
			&CLSID_NULL,        // Connects to filter
			NULL,	            // Connects to pin
			1,                  // Number of types
			&sudPinTypes        // The pin details
		}
};


const AMOVIESETUP_FILTER sudDebugInfoFilter =
{
	&CLSID_DebugInfoFilter, // Filter CLSID
	FILTER_NAME,     // Filter name
	MERIT_DO_NOT_USE,       // Its merit
	//MERIT_PREFERRED + 0xf8888,       // Its merit
	2,                      // Number of pins
	psudPins                // Pin details
};

CFactoryTemplate g_Templates[] = {
	{ 
		FILTER_NAME,
		&CLSID_DebugInfoFilter,
		CDebugInfoFilter::CreateInstance,
		NULL,
		&sudDebugInfoFilter
	},
    { 
        L"Debug Info Filter Property", 
        &CLSID_DebugInfoFilterProperty, 
		CDebugInfoFilterProperty::CreateInstance,
        NULL, 
        NULL 
    },
	{ 
		L"Graph Info Property", 
		&CLSID_GraphInfoProperty, 
		CGraphInfoPropertyPage::CreateInstance,
		NULL, 
		NULL 
	},
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


STDAPI DllRegisterServer()
{
	LPCTSTR pszCommandLine = GetCommandLine();
	return AMovieDllRegisterServer2( TRUE );
}


STDAPI DllUnregisterServer()
{
	LPCTSTR pszCommandLine = GetCommandLine();
	return AMovieDllRegisterServer2( FALSE );
}
