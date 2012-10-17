#include "StdAfx.h"
#include "DShowUtility.h"

HRESULT CDShowUtility::GetFirstUnConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    *ppPin = NULL;

    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);

    if (FAILED(hr))
    {
        return hr;
    }
    pEnum->Reset();

    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);

        if (PinDir == ThisPinDir)
        {
            IPin *pTmp = 0;
            hr = pPin->ConnectedTo(&pTmp);
            
            if (NULL != pTmp)
            {
                pTmp->Release();
            }

            if (SUCCEEDED(hr))
            {
                //已经连接了
                pPin->Release();
                continue;
            }
            else
            {
                pEnum->Release();
                *ppPin = pPin;

                return S_OK;
            }
        }

        pPin->Release();

    }

    pEnum->Release();

    return E_FAIL;
}

HRESULT CDShowUtility::ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest)
{
    if (pGraph == NULL || pOut == NULL || pDest == NULL)
    {
        return E_POINTER;
    }

    IPin *pIn = NULL;
    HRESULT hr = GetFirstUnConnectedPin(pDest, PINDIR_INPUT, &pIn);

    if (FAILED(hr))
    {
        return hr;
    }

    // 将输出pin 连接到输入pin
    hr = pGraph->Connect(pOut, pIn);
    pIn->Release();

    return hr;
}

HRESULT CDShowUtility::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest)
{
    if (pGraph == NULL || pSrc == NULL || pDest == NULL)
    {
        return E_POINTER;
    }

    // Find an output pin on the first filter
    IPin *pOut = NULL;
    HRESULT hr = GetFirstUnConnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (FAILED(hr))
    {
        return hr;
    }
    hr = ConnectFilters(pGraph, pOut, pDest);
    pOut->Release();
    return hr;
}

HRESULT CDShowUtility::FindFilterInterface(IGraphBuilder *pGraph, REFGUID iid, void **ppUnk)
{
    if (!pGraph || !ppUnk) 
    {
        return E_POINTER;
    }

    HRESULT hr = E_FAIL;
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pF = NULL;

    if (FAILED(pGraph->EnumFilters(&pEnum)))
    {
        return E_FAIL;
    }

    //分别调用 Queryinterface, 枚举filter Graph 中的所有Filter 
    while (S_OK == pEnum->Next(1, &pF, 0))
    {
        hr = pF->QueryInterface(iid, ppUnk);
        pF->Release();

        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    pEnum->Release();
    return hr;    
}

HRESULT CDShowUtility::GetNextFilter(IBaseFilter *pFilter, PIN_DIRECTION Dir, IBaseFilter **ppNext)
{
    if (!pFilter || !ppNext)
    {
        return E_POINTER;
    }

    IEnumPins *pEnum = NULL;
    IPin *pPin = 0;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    while (S_OK == pEnum->Next(1, &pPin, 0))
    {
        // see if this pin matches the specified direction
        PIN_DIRECTION ThisPinDir;
        hr = pPin->QueryDirection(&ThisPinDir);
        if (FAILED(hr))
        {
            // something strange happened 
            hr = E_UNEXPECTED;
            pPin->Release();
            break;
        }

        if (ThisPinDir == Dir)
        {
            // check if the pin is connected to another pin
            IPin *pPinNext = NULL;

            if (SUCCEEDED(hr))
            {
                // get the filter that owns that pin
                PIN_INFO PinInfo;
                hr = pPinNext->QueryPinInfo(&PinInfo);
                pPinNext->Release();
                pEnum->Release();
                if (FAILED(hr) || PinInfo.pFilter == NULL)
                {
                    // Something strange happened 
                    return E_UNEXPECTED;
                }

                // This is the filter we're looking for 
                *ppNext = PinInfo.pFilter;
                return S_OK;
            }
        }

        pPin->Release();
    }
    pEnum->Release();

    return hr;
}
