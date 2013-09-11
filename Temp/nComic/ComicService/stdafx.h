// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "resource.h"
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>

#include <atlcom.h>
#include <atlctl.h>
#include <atlcoll.h>

#include <atlcrack.h>

using namespace ATL;

#include <ftlbase.h>
#include <ftlThread.h>
//#include <ftlService.h>
#include <ftlSystem.h>
