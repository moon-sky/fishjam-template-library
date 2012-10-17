#pragma once

#include <strmif.h>

class CDShowUtility
{
public:
    static HRESULT GetFirstUnConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

    static HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest);
    static HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest);
    static HRESULT FindFilterInterface(IGraphBuilder *pGraph, REFGUID iid, void **ppUnk);

    static HRESULT GetNextFilter(IBaseFilter *pFilter, PIN_DIRECTION Dir, IBaseFilter **ppNext);

};
