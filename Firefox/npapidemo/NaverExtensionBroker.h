#pragma once

#include <string>
#include "ScriptablePluginObjectBase.h"
//#include "NaverLogManager.h"
//#include "DDBBitmap.h"
//#include "Thread.h"
//#include "ntbdownloader_define.h"
//#include "IPCServer.h"
//#include "MsgWnd.h"

using namespace std;

class NaverExtensionBroker : public ScriptablePluginObjectBase //ConstructablePluginObject
{
public:
	NaverExtensionBroker(NPP npp);
	virtual ~NaverExtensionBroker(void);

	DECLARE_EXPORT_METHOD_MAP(NaverExtensionBroker);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);

	virtual void OnEventCall(LPCTSTR sContentsURL, int nType, int nErrorCode, LPCTSTR sMsg);

	virtual bool Deallocate();

	void CallBackFunc(int itype,std::string &path);

	void	ShowCapturePreview(const std::string& bitmapPath);

	BOOL    OnMsgReceive(UINT Message,WPARAM wParam,LPARAM lParam);
	static unsigned int __stdcall myTestThread(void* pParam);
protected:
	DECLARE_EXPORT_METHOD(Initialize);

	DECLARE_EXPORT_METHOD(OutputLog);

	DECLARE_EXPORT_METHOD(CaptureRegion);
	DECLARE_EXPORT_METHOD(CaptureWindow);
	DECLARE_EXPORT_METHOD(CaptureAllDocument);
	DECLARE_EXPORT_METHOD(CaptureShowDocument);
	DECLARE_EXPORT_METHOD(SaveCaptureImage);
	DECLARE_EXPORT_METHOD(ChangeInfo);

	DECLARE_EXPORT_METHOD(StartUtility);
	DECLARE_EXPORT_METHOD(Is64BitOS);
	DECLARE_EXPORT_METHOD(SelectDir);
	DECLARE_EXPORT_METHOD(SyncPreferences);

	DECLARE_EXPORT_METHOD(OnActiveChaged)
	DECLARE_EXPORT_METHOD(onCreated);
	DECLARE_EXPORT_METHOD(onBeforeNavigate);
	DECLARE_EXPORT_METHOD(onCommitted);
	DECLARE_EXPORT_METHOD(onCompleted);
	DECLARE_EXPORT_METHOD(onClickFun);
	DECLARE_EXPORT_METHOD(onTest);
	DECLARE_EXPORT_METHOD(GetDefaultPicturePath);

protected:
	HWND	GetActiveBrowser();	
	
	//void	ShowCapturePreview(const DDBBitmap& bitmap);

	bool	Start();
	bool	Stop();

	void EnableGesture(bool enable);

	BOOL WriteMem(const WCHAR *userID,const WCHAR *userHash,const WCHAR *cookie,const WCHAR *url,const WCHAR *targetPath, int isUpload,const WCHAR *videoDomain,const WCHAR *videoCookie);
	BOOL RunExe_ex();
    
	void    ShowCaptureDocment();
	int     ChangCode(char *buf);
	bool    SetCommandList();
public:
	NPP					m_pNPInstance;
	HWND				m_activeWindow;
	//IPCServer			m_server;
	BOOL				m_bGestureEnabled;

	//CNaverLogManager	m_logManager;
	BOOL				m_bAnableSendLog;	

	std::wstring		m_szDownloaderPath;
	NPObject            *m_pJsCallBackObj;
	//MwgWnd              m_MsgWnd;

	std::string     m_sDefaultFilePath;
	std::string     m_PicFormart;
	int             m_indexNameFormart;       

};

