#ifndef IE_EXTENSION_SUPPORT_H
#define IE_EXTENSION_SUPPORT_H
#pragma once

class CIEExtensionSupport
{
public:
    //从浏览器的工具栏中获取 IE 浏览器的指针,通过 IObjectWithSite 的(SetSite)(IUnknown* pUnkSite)方法
    static HRESULT DemoGetWebBrowserInDeskBand(IUnknown* pUnkSite)
    {
        //Get the parent window.
        IOleWindow  *pOleWindow = NULL;
        HWND hWndParent = NULL;
        if(SUCCEEDED(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
        {
            pOleWindow->GetWindow(&hWndParent);
            pOleWindow->Release();
        }
        //Get and keep the IInputObjectSite pointer.
        IInputObjectSite* pSite = NULL;
        pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&pSite);

        //获取WebBrower接口
        IWebBrowser2* s_pFrameWB = NULL;
        IOleCommandTarget* pCmdTarget = NULL;
        HRESULT hr = pUnkSite->QueryInterface(IID_IOleCommandTarget, (LPVOID*)&pCmdTarget);
        if (SUCCEEDED(hr))
        {
            IServiceProvider* pSP;
            hr = pCmdTarget->QueryInterface(IID_IServiceProvider, (LPVOID*)&pSP);

            pCmdTarget->Release();

            if (SUCCEEDED(hr))
            {
                hr = pSP->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (LPVOID*)&s_pFrameWB);
                pSP->Release();
                _ASSERT(s_pFrameWB);
                //使用接口
                s_pFrameWB->Release();
                s_pFrameWB = NULL;
            }
        }
    }
};
#endif //IE_EXTENSION_SUPPORT_H