/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


//////////////////////////////////////////////////
//
// CPlugin class implementation
//

#include "stdafx.h"

#include <stdio.h>
#include "plugin.h"
#include "npfunctions.h"

#include "ExtensionBroker.h"

#include <windowsx.h>


#define PR_TRUE		true
#define PR_FALSE	false

#define     SubclassWindow(hwnd, lpfn)       \
              ((WNDPROC)SetWindowLongPtr((hwnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))

static NPIdentifier sFoo_id;
static NPIdentifier sBar_id;
static NPIdentifier sDocument_id;
static NPIdentifier sBody_id;
static NPIdentifier sCreateElement_id;
static NPIdentifier sCreateTextNode_id;
static NPIdentifier sAppendChild_id;
static NPIdentifier sPluginType_id;


//////////////////////////////////////////////////////////////////////////

static NPObject *
AllocateExtensionBroker(NPP npp, NPClass *aClass)
{
	return new ExtensionBroker(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ExtensionBroker, AllocateExtensionBroker);
//////////////////////////////////////////////////////////////////////////


CPlugin::CPlugin(NPP pNPInstance) :
m_pNPInstance(pNPInstance),
m_pNPStream(NULL),
m_bInitialized(FALSE),
m_pScriptableObject(NULL)
{
	FUNCTION_BLOCK_TRACE(0);
	//MessageBox(NULL, TEXT("CPlugin::CPlugin"), NULL, MB_OK);
#ifdef XP_WIN
	m_hWnd = NULL;
#endif
}

CPlugin::~CPlugin()
{
	//MessageBox(NULL, TEXT("CPlugin::~CPlugin"), NULL, MB_OK);

	if (m_pScriptableObject)
	{
		NPN_ReleaseObject(m_pScriptableObject);
	}

}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif

NPBool CPlugin::init(NPWindow* pNPWindow)
{
	if(pNPWindow == NULL)
	{
		return FALSE;
	}

#ifdef XP_WIN
	m_hWnd = (HWND)pNPWindow->window;
	if(m_hWnd == NULL)
	{
		return FALSE;
	}

	// subclass window so we can intercept window messages and
	// do our drawing to it
	lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);

	// associate window with our CPlugin object so we can access 
	// it in the window procedure
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
#endif

	m_Window = pNPWindow;

	m_bInitialized = TRUE;
	return TRUE;
}

void CPlugin::shut()
{

#ifdef XP_WIN
	// subclass it back
	SubclassWindow(m_hWnd, lpOldProc);
	m_hWnd = NULL;
#endif

	m_bInitialized = FALSE;
}

NPBool CPlugin::isInitialized()
{
	return m_bInitialized;
}

int16_t CPlugin::handleEvent(void* event)
{
	NPEvent* ev = (NPEvent*)event;
	if (ev)
	{
		FTLTRACE(TEXT("CPlugin::handleEvent event = %d(0x%x)\n"), ev->event, ev->event);
	}
#ifdef XP_MAC
	NPEvent* ev = (NPEvent*)event;
	if (m_Window) {
		Rect box = { m_Window->y, m_Window->x,
			m_Window->y + m_Window->height, m_Window->x + m_Window->width };
		if (ev->what == updateEvt) {
			::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
		}
	}
#endif
	return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
	const char *ua = NPN_UserAgent(m_pNPInstance);
	strcpy_s(m_String, 128, ua);

#ifdef XP_WIN
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
#endif

	if (m_Window) {
		NPRect r =
		{
			(uint16_t)m_Window->y, (uint16_t)m_Window->x,
			(uint16_t)(m_Window->y + m_Window->height),
			(uint16_t)(m_Window->x + m_Window->width)
		};

		NPN_InvalidateRect(m_pNPInstance, &r);
	}
}

// this will clean the plugin window
void CPlugin::clear()
{
	strcpy_s(m_String, 128, "");

#ifdef XP_WIN
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
#endif

}

void CPlugin::getVersion(char* *aVersion)
{
	const char *ua = NPN_UserAgent(m_pNPInstance);
	char*& version = *aVersion;
	version = (char*)NPN_MemAlloc(1 + strlen(ua));
	if (version)
	{
		strcpy_s(version, 1 + strlen(ua), ua);
	}
}

NPObject *
CPlugin::GetScriptableObject()
{
	if (!m_pScriptableObject) 
	{
		m_pScriptableObject = NPN_CreateObject(m_pNPInstance, GET_NPOBJECT_CLASS(ExtensionBroker));

		NPVariant v;
		NPIdentifier n = NPN_GetStringIdentifier("Instance");

		INT32_TO_NPVARIANT((int32_t)m_pNPInstance, v);
		m_pScriptableObject->_class->setProperty(m_pScriptableObject, n, &v);
	}

	if (m_pScriptableObject) 
	{
		NPN_RetainObject(m_pScriptableObject);
	}

	return m_pScriptableObject;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
    case WM_PAINT:
      {
        // draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
				FillRect( hdc, &rc, (HBRUSH) (COLOR_WINDOW+1));
        FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));
        CPlugin * p = (CPlugin*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if(p) {
					//kk: some fancy GUI stuff
          /*if (p->m_String[0] == 0) {
            strcpy("foo", p->m_String);
          }*/
		//char *s = p->GetGuiText();
		

          DrawText(hdc, TEXT("In CPluginPaint"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
      }
      break;
    default:
      break;
  }
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
