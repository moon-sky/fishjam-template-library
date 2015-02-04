// AtlServiceDemoObject.cpp : Implementation of CAtlServiceDemoObject

#include "stdafx.h"
#include "AtlServiceDemoObject.h"
// CAtlServiceDemoObject

STDMETHODIMP CAtlServiceDemoObject::Multi(LONG a, LONG b, LONG* pResult)
{
    *pResult = a * b;
    return S_OK;
}
