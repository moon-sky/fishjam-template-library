#include "StdAfx.h"
#include "MFTranscode.h"

CMFTranscode::CMFTranscode(void)
{
}

CMFTranscode::~CMFTranscode(void)
{
}

HRESULT CMFTranscode::OpenSrcFile(LPCTSTR pszSrcURL)
{
    HRESULT hr = S_OK;

    // Create the media source.
    MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

    CComPtr<IMFSourceResolver> spSourceResolver;
    CComPtr<IUnknown> spUnkSource;

    // Create the source resolver.
    COM_VERIFY(MFCreateSourceResolver(&spSourceResolver));
    if (spSourceResolver)
    {
        // Use the source resolver to create the media source.
        COM_VERIFY(spSourceResolver->CreateObjectFromURL(
            pszSrcURL,                       // URL of the source.
            MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
            NULL,                       // Optional property store.
            &ObjectType,                // Receives the created object type. 
            &spUnkSource                 // Receives a pointer to the media source.
            ));

        // Get the IMFMediaSource from the IUnknown pointer.
        if (SUCCEEDED(hr))
        {
            COM_VERIFY(spUnkSource->QueryInterface(IID_IMFMediaSession, (void**)&m_spMediaSession));
        }
    }

    //Create the media session.
    if (SUCCEEDED(hr))
    {
        COM_VERIFY(MFCreateMediaSession(NULL, &m_spMediaSession));
    }    

    if (SUCCEEDED(hr))
    {
        // Create an empty transcode profile.
        COM_VERIFY(MFCreateTranscodeProfile(&m_spTranscodeProfile));
    }
    return hr;
}