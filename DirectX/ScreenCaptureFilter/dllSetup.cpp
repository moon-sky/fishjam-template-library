#include "stdafx.h"
#include "ScreenCaptureFilter.h"

//extern const AMOVIESETUP_FILTER sudFishjamScreenCapture;

// Filter setup data
const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
{
	&MEDIATYPE_Video,       // Major type
	&MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudOutputPin =
{ 
	L"Output",      // Obsolete, not used.
	FALSE,          // Is this pin rendered?
	TRUE,           // Is it an output pin?
	FALSE,          // Can the filter create zero instances?
	FALSE,          // Does the filter create multiple instances?
	&CLSID_NULL, // PIN_CATEGORY_CAPTURE,    // clsConnectsToFilter.
	NULL,           // Obsolete.
	1,              // Number of media types.
	&sudOpPinTypes  // Pointer to media types.
};

const AMOVIESETUP_FILTER sudFishjamScreenCapture =
{
	&CLSID_FishjamScreenCapture,// Filter CLSID
	L"Fishjam Screen Capture",  // String name
	MERIT_DO_NOT_USE,           // Filter merit
	1,                          // Number pins
	&sudOutputPin               // Pin details
};

// COM global table of objects in this dll
CFactoryTemplate g_Templates[] =
{
	{
		L"Fishjam Screen Capture",				// Name
		&CLSID_FishjamScreenCapture,			// CLSID
		CScreenCaptureFilter::CreateInstance,	// Method to create an instance of MyComponent
		NULL,									// Initialization function
		&sudFishjamScreenCapture				// Set-up information (for filters)
	},
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

REGFILTER2 rf2FilterReg = {
	1,					// Version 1 (no pin mediums or pin category).
	MERIT_DO_NOT_USE,   // Merit.
	1,					// Number of pins.
	&sudOutputPin		// Pointer to pin information.
};

STDAPI DllRegisterServer()
{ 
	HRESULT hr = E_FAIL;
	COM_VERIFY(AMovieDllRegisterServer2( TRUE ));

#if 0
	if (SUCCEEDED(hr))
	{
		CComPtr<IFilterMapper2> spFilterMapper2;
		COM_VERIFY(CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC,
			IID_IFilterMapper2, (void **)&spFilterMapper2));
		if (spFilterMapper2)
		{
			COM_VERIFY(spFilterMapper2->RegisterFilter(
				CLSID_FishjamScreenCapture,         // Filter CLSID. 
				L"Fishjam Screen Capture",          // Filter name.
				NULL,								// Device moniker. 
				&CLSID_VideoInputDeviceCategory,	// Video Input category.
				L"Fishjam Screen Capture",                       // Instance data.
				&rf2FilterReg                    // Pointer to filter information.
				));
		}
	}
#endif

	return hr;
}


STDAPI DllUnregisterServer()
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(AMovieDllRegisterServer2( FALSE ));

#if 0
	if (SUCCEEDED(hr))
	{
		CComPtr<IFilterMapper2> spFilterMapper2;
		COM_VERIFY(CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC,
			IID_IFilterMapper2, (void **)&spFilterMapper2));
		if (spFilterMapper2)
		{
			COM_VERIFY(spFilterMapper2->UnregisterFilter(
				&CLSID_VideoInputDeviceCategory,	// Video Input category.
				L"Fishjam Screen Capture",                       // Instance data.
				CLSID_FishjamScreenCapture         // Filter CLSID. 
				));
		}
	}
#endif
	return hr;
}