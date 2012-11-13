#include "stdafx.h"

#include "ColourExFilter.h"

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


const AMOVIESETUP_FILTER sudColourExFilter =
{
	&CLSID_ColourExFilter, // Filter CLSID
	COLOUREX_FILTER_NAME,     // Filter name
	MERIT_DO_NOT_USE,       // Its merit
	2,                      // Number of pins
	psudPins                // Pin details
};

CFactoryTemplate g_Templates[] = {
	{ 
		COLOUREX_FILTER_NAME,
		&CLSID_ColourExFilter,
		CColourExFilter::CreateInstance,
		NULL,
		&sudColourExFilter
	}
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}


STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}
