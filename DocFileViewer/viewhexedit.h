#pragma once 

#include "resource.h"       // main symbols 
#include <atlddx.h>

#define  ID_DISPATCH_HEXEDIT    10003 

class CViewHexEdit
    : public CWindowImpl<CViewHexEdit> 
    , public CMessageFilter 
    , public IDispEventImpl<ID_DISPATCH_HEXEDIT, CViewHexEdit> 
{ 
public: 
    CViewHexEdit(void); 
    ~CViewHexEdit(void); 

    typedef CWindowImpl<CViewHexEdit> baseClass; 
public: 

    virtual BOOL PreTranslateMessage(MSG* pMsg); 

    DECLARE_WND_CLASS(_T("CViewHexEdit")) 

    BEGIN_MSG_MAP(CViewHexEdit) 
        MESSAGE_HANDLER(WM_CREATE, OnWmCreate) 
        MESSAGE_HANDLER(WM_DESTROY, OnWmDestroy) 
        MESSAGE_HANDLER(WM_SIZE, OnWmSize) 
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow) 
    END_MSG_MAP() 

    LRESULT OnWmCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 

    BEGIN_SINK_MAP(CViewHexEdit) 
        //SINK_ENTRY(ID_DISPATCH_HEXEDIT, 0x01, SagePreviewOnClose) 
        //SINK_ENTRY(ID_DISPATCH_HEXEDIT, 0x02, SagePreviewOnPreviewReady) 
        //SINK_ENTRY(ID_DISPATCH_HEXEDIT, 0x03, ShImgVwOnError) 
        //SINK_ENTRY(ID_DISPATCH_HEXEDIT, 0x04, ShImgVwOnBestFitPress) 
        //SINK_ENTRY(ID_DISPATCH_HEXEDIT, 0x05, ShImgVwOnActualSizePress) 
    END_SINK_MAP()

    //void __stdcall SagePreviewOnClose(); 
    //void __stdcall SagePreviewOnPreviewReady(); 
    //void __stdcall ShImgVwOnError(); 
    //void __stdcall ShImgVwOnBestFitPress(); 
    //void __stdcall ShImgVwOnActualSizePress(); 

    BOOL SetReadOnly(BOOL bReadOnly); 
    BOOL SetData(const VARIANT & vData); 

private: 
    CComQIPtr<HexEditor::IHexEdit> m_spHexEdit; 
    //CComQIPtr<Flash::IShockwaveFlash> m_spHexEdit; 
    CAxWindow m_wndAxHost; 

private: 
    //TCHAR m_szBmpFile[MAX_PATH]; 
};
