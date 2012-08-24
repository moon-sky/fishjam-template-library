#include "stdafx.h"
#include "NaverExtensionBroker.h"
#include "Tchar.h"
#include "ftlnpAPI.h"
#include <ShellAPI.h>
#include <sstream>
#include <fstream>
#include <psapi.h>
#include <ShellAPI.h>
#include <Shlobj.h>
#include <WinInet.h>

//#include "NaverHook.h"
//
//#include "Log.h"
//#include "ApplicationConfig.h"
//#include "WindowUtil.h"
//#include "StringUtil.h"
//#include "Util.h"
//#include "ScreenCapture.h"
//#include "../CommonLibs/Base64.h"
//#include "ByteArray.h"
//#include "StringUtil.h"

NaverExtensionBroker::NaverExtensionBroker(NPP npp)
	: ScriptablePluginObjectBase(npp) //ConstructablePluginObject(npp) //
	, m_activeWindow(NULL)
//	, m_server(this)
	, m_bAnableSendLog(true)
{
	FUNCTION_BLOCK_TRACE(0);
	INIT_EXPORT_METHOD_MAP();

	m_pJsCallBackObj = NULL;
	m_bGestureEnabled = FALSE;
	//MessageBox(NULL, TEXT("NaverExtensionBroker::NaverExtensionBroker"), NULL, MB_OK);

}

NaverExtensionBroker::~NaverExtensionBroker(void)
{
	FUNCTION_BLOCK_TRACE(0);
	//MessageBox(NULL, TEXT("NaverExtensionBroker::~NaverExtensionBroker"), NULL, MB_OK);
	Stop();
}

BEGIN_EXPORT_METHOD_MAP(NaverExtensionBroker)
	REGISTER_EXPORT_METHOD("Initialize",			&NaverExtensionBroker::Initialize);	

	REGISTER_EXPORT_METHOD("OutputLog",				&NaverExtensionBroker::OutputLog);
	REGISTER_EXPORT_METHOD("CaptureRegion",			&NaverExtensionBroker::CaptureRegion);
	REGISTER_EXPORT_METHOD("CaptureWindow",			&NaverExtensionBroker::CaptureWindow);
	REGISTER_EXPORT_METHOD("CaptureAllDocument",	&NaverExtensionBroker::CaptureAllDocument);
	REGISTER_EXPORT_METHOD("CaptureShowDocument",	&NaverExtensionBroker::CaptureShowDocument);
	REGISTER_EXPORT_METHOD("SaveCaptureImage",		&NaverExtensionBroker::SaveCaptureImage);
	REGISTER_EXPORT_METHOD("GetDefaultPicturePath",	&NaverExtensionBroker::GetDefaultPicturePath);

	REGISTER_EXPORT_METHOD("ChangeInfo",			&NaverExtensionBroker::ChangeInfo);
	REGISTER_EXPORT_METHOD("StartUtility",			&NaverExtensionBroker::StartUtility);
	REGISTER_EXPORT_METHOD("Is64BitOS",				&NaverExtensionBroker::Is64BitOS);
	REGISTER_EXPORT_METHOD("SelectDir",				&NaverExtensionBroker::SelectDir);
	REGISTER_EXPORT_METHOD("SyncPreferences",		&NaverExtensionBroker::SyncPreferences);

	REGISTER_EXPORT_METHOD("OnActiveChaged",		&NaverExtensionBroker::OnActiveChaged);
	REGISTER_EXPORT_METHOD("onCreated",				&NaverExtensionBroker::onCreated);
	REGISTER_EXPORT_METHOD("onBeforeNavigate",		&NaverExtensionBroker::onBeforeNavigate);	
	REGISTER_EXPORT_METHOD("onCommitted",			&NaverExtensionBroker::onCommitted);	
	REGISTER_EXPORT_METHOD("onCompleted",			&NaverExtensionBroker::onCompleted);	
	REGISTER_EXPORT_METHOD("onclick",   			&NaverExtensionBroker::onClickFun);	
	REGISTER_EXPORT_METHOD("test",   			    &NaverExtensionBroker::onTest);	

END_EXPORT_METHOD_MAP()

bool NaverExtensionBroker::HasMethod(NPIdentifier name)
{
	std::string strName = NPN_UTF8FromIdentifier(name);
	FTLTRACE(TEXT("%s, name=%s\n"), TEXT(__FUNCTION__), CFConversion().UTF8_TO_TCHAR(strName.c_str()));

	FTL::FormatMessageBox(NULL, TEXT("NaverExtensionBroker::HasMethod"),MB_OK, 
		TEXT("name=%s"),  CFConversion().UTF8_TO_TCHAR(strName.c_str()));

	if(NPN_IdentifierIsString(name))
	{
		std::string strFunc = NPN_UTF8FromIdentifier(name);
		bool bret = TEST_EXPORT_METHOD(strFunc);
		return bret;
	}
	return false;
}

bool NaverExtensionBroker::HasProperty(NPIdentifier name)
{
	std::string strPropName = NPN_UTF8FromIdentifier(name);
	FTLTRACE(TEXT("%s, name=%s\n"), TEXT(__FUNCTION__), CFConversion().UTF8_TO_TCHAR(strPropName.c_str()));

	if(strPropName == "m_hWnd")
	{
		return true;
	}
	else if(strPropName == "Instance")
	{
		return true;
	}
	else if(strPropName == "OnEnter")
	{
		return true;
	}
	else if(strPropName == "onclick")
	{
		return true;
	}
	return false;
}
bool NaverExtensionBroker::Deallocate()
{
	FUNCTION_BLOCK_TRACE(0);
	if(m_pJsCallBackObj)  
	{  
		NPN_ReleaseObject(m_pJsCallBackObj);  
		m_pJsCallBackObj=NULL;  
	}

	return false;
}
bool NaverExtensionBroker::GetProperty(NPIdentifier name, NPVariant *result)
{
	FUNCTION_BLOCK_TRACE(0);
	std::string strPropName = NPN_UTF8FromIdentifier(name);
	FTLTRACE(TEXT("%s, name=%s\n"), TEXT(__FUNCTION__), CFConversion().UTF8_TO_TCHAR(strPropName.c_str()));
	return false;
}

bool NaverExtensionBroker::SetProperty(NPIdentifier name, const NPVariant *pValue)
{
	std::string strPropName = NPN_UTF8FromIdentifier(name);
	FTLTRACE(TEXT("%s, name=%s\n"), TEXT(__FUNCTION__), CFConversion().UTF8_TO_TCHAR(strPropName.c_str()));

	if(strPropName == "Instance")
	{
		m_pNPInstance = (NPP)(pValue->value.intValue);
		return true;
	}
	else if (strPropName == "OnEnter")
	{
		if (!m_pJsCallBackObj)
		{
			m_pJsCallBackObj = NPN_RetainObject(NPVARIANT_TO_OBJECT(*pValue));
			return true;
		}
	}
	else if(strPropName == "onclick")
	{
		if (!m_pJsCallBackObj)
		{
			m_pJsCallBackObj = NPN_RetainObject(NPVARIANT_TO_OBJECT(*pValue));
			return true;
		}
	}
	return false;
}

bool NaverExtensionBroker::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	FUNCTION_BLOCK_TRACE(0);
	if(NPN_IdentifierIsString(name))
	{
		std::string strFunc = NPN_UTF8FromIdentifier(name);
		return INVOKE_EXPORT_METHOD(strFunc, args, argCount, result);
	}

	return false;
}

bool NaverExtensionBroker::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	FUNCTION_BLOCK_TRACE(0);
	return true;
}

bool NaverExtensionBroker::Initialize(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	FUNCTION_BLOCK_TRACE(0);
	BOOL _retval = FALSE;
	//Log::OutPut(L"NaverExtensionBroker::Initialize");

	if (argCount == 1)
	{
		NPString npPrefs = NPVARIANT_TO_STRING(args[0]);
		if( INTERNET_MAX_URL_LENGTH < npPrefs.UTF8Length)
		{
			return true;
		}
		std::string szPrefs(npPrefs.UTF8Characters, npPrefs.UTF8Length);
		//ApplicationConfig::Instance();
		//ApplicationConfig::Instance().Init(szPrefs, true);
	}

	NPVariant temp;
	_retval = Start();

	//if (ApplicationConfig::Instance().GetBool("gesture_isUsing", true))
	//	EnableGesture(true);
	//else 
	//	EnableGesture(false);

	OnActiveChaged(NULL, 0, &temp);

	//m_MsgWnd.Create(GetModuleHandle(L"npNaverExtension.dll"),this);

//	MessageBox(NULL,L"Initialize start",L"tip",MB_OK);

	SetCommandList();
	
	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

void NaverExtensionBroker::EnableGesture(bool enable)
{
	//if (enable)
	//	SetHookType(HOOKTYPE_GESTURE);
	//else
	//	SetHookType(HOOKTYPE_NOTHING);
}

bool NaverExtensionBroker::Start()
{
	//if (m_bGestureEnabled)
	//	return true;

	//m_server.StartCertainly();
	//if (!StartHook())
	//{
	//	m_server.Terminate();
	//	m_server.Wait(3000);
	//	return false;
	//}
	//m_bGestureEnabled = TRUE;
	return true;
}

bool NaverExtensionBroker::Stop()
{
	//if (m_bGestureEnabled == FALSE)
	//	return true;

	//StopHook();

	//m_server.Terminate();
	//m_server.Wait(3000);
	//m_bGestureEnabled = FALSE;
	return true;
}

bool NaverExtensionBroker::OutputLog(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	//MessageBox(NULL, TEXT("In NaverExtensionBroker::OutputLog"), NULL, MB_OK);
	return true;

	BOOL _retval = TRUE;

	NPString log = NPVARIANT_TO_STRING(args[0]);
	std::string strLog(log.UTF8Characters, log.UTF8Length);
	//Log::OutPut(strLog);

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::CaptureRegion(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	//SetActiveBrowser(GetActiveBrowser());
	//SetHookType(HOOKTYPE_CAPTURE_REGION);
	
	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::CaptureWindow(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	//SetHookType(HOOKTYPE_CAPTURE_WINDOW);
		
	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::CaptureAllDocument(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

			
	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::SaveCaptureImage(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	NPString argument = NPVARIANT_TO_STRING(args[0]);
	std::string data(argument.UTF8Characters, argument.UTF8Length);
	std::string::size_type ts = data.find(";");
	std::string::size_type cs = data.find(",");
	std::string type = data.substr(0, ts);
	std::string crypt = data.substr(ts + 1, cs - ts - 1);
	data = data.substr(cs + 1);

	//// Decrypt
	//ByteArray by = BBase64::Decrypt(data.c_str(), data.size());

	//// save
	//std::wstring path = Util::GetTempPath(L"Naver");
	//path.append(L"naverCapture.png");
	//std::fstream file;
	//file.open(path.c_str(), std::fstream::binary | std::fstream::out | std::fstream::trunc);

	//if (file.is_open())
	//{
	//	file.write(by.Data(), by.Length());
	//	file.close();

	//	DDBBitmap bmp;
	//	bmp.LoadPng(path);
	//	ShowCapturePreview(bmp);
	//}

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::CaptureShowDocument(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	//m_MsgWnd.SendMsg(UM_SHOWCAPTUREDOCMENT,NULL,NULL,FALSE);

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}
BOOL  NaverExtensionBroker::OnMsgReceive(UINT Message,WPARAM wParam,LPARAM lParam)
{
	switch(Message)
	{
	case UM_SHOWCAPTUREDOCMENT:
		{
			ShowCaptureDocment();
			break;
		}

		return TRUE;
	}
	return FALSE;
}
void    NaverExtensionBroker::ShowCaptureDocment()
{
	//Sleep(500);
	//std::wstring path = Util::GetTempPath(L"Naver");
	//path.append(L"naverCapture.bmp");

	//HWND activeBrowser = GetActiveBrowser();
	////DDBBitmap bitmap = ScreenCapture::Capture(activeBrowser);

	//ShowCapturePreview(bitmap);
}
bool NaverExtensionBroker::OnActiveChaged(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	//Log::OutPut(L">>>>>>>>>> [NaverExtensionBroker::OnActiveChaged 1]");

	m_activeWindow = ::FindWindowEx(NULL, NULL, TEXT("Chrome_WidgetWin_1") , NULL);
	while (m_activeWindow != NULL) 
	{
		TCHAR title[MAX_PATH] = { 0 };
		GetWindowText(m_activeWindow, title, MAX_PATH);
		if (_tcsstr(title, TEXT("Google Chrome")) == 0)
		{
			m_activeWindow = ::FindWindowEx(NULL, m_activeWindow, TEXT("Chrome_WidgetWin_1") , NULL);
		}
		else
			break;
	}

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

HWND NaverExtensionBroker::GetActiveBrowser()
{
	HWND rtn = NULL;
	if (m_activeWindow)
	{
 		RECT rect;
 		::GetWindowRect(m_activeWindow, &rect);
 		POINT browserPoint = {rect.left + 200, rect.top + 250};
 		rtn = ::WindowFromPoint(browserPoint);
	}
	return rtn;
}

void NaverExtensionBroker::ShowCapturePreview(const std::string& bitmapPath)
{
#if _DEBUG
	HINSTANCE instance = GetModuleHandle(TEXT("npNaverExtension.dll"));
#else
	HINSTANCE instance = GetModuleHandle(TEXT("npNaverExtension.dll"));
#endif	

	//DDBBitmap bitmap;
	//if (bitmap.Load(instance, StringUtil::Utf8ToWString(bitmapPath)))
	//{
	//	ShowCapturePreview(bitmap);
	//}
}

//#include "CapturePreview.h"
//void NaverExtensionBroker::ShowCapturePreview(const DDBBitmap& bitmap)
//{
//	//CapturePreview dlg(m_activeWindow, bitmap);
//	//dlg.SetValue(m_PicFormart,m_sDefaultFilePath,m_indexNameFormart);
//	//dlg.DoModal(this);
//}

bool NaverExtensionBroker::onCreated(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	NPString url = NPVARIANT_TO_STRING(args[0]);
	if( INTERNET_MAX_URL_LENGTH < url.UTF8Length )
	{
		return true;
	}

	int iSize = ( url.UTF8Length + 1);
	NPUTF8 *szUrl = new NPUTF8[iSize];
	memset( szUrl, 0, iSize );

	strncpy( szUrl, url.UTF8Characters, url.UTF8Length );
	//Log::OutPut("----------------------------[onCreate]-------------------------------");
	//Log::OutPut(szUrl);

	delete []szUrl;


	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::ChangeInfo(const NPVariant *args, uint32_t argCount, NPVariant *result)
{

	BOOL _retval = TRUE;


	NPString url = NPVARIANT_TO_STRING(args[0]);
	if( INTERNET_MAX_URL_LENGTH < url.UTF8Length )
	{
		return true;
	}	

	std::string szUrl(url.UTF8Characters, url.UTF8Length);
	//Log::OutPut( "----------------------------[ChangeInfo]-------------------------------" );
	
	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}


typedef BOOL (WINAPI *PQueryFullProcessImageName)(
	__in     HANDLE hProcess,
	__in     DWORD dwFlags,
	__out    LPTSTR lpExeName,
	__inout  PDWORD lpdwSize
	);

typedef struct ProcessRunningInfo
{
	LPCTSTR szFilePath;
	BOOL bFound;
	BOOL bVistaOrLater;
	PQueryFullProcessImageName pQueryFullProcessImageName;
}*LPProcessRunningInfo;

#if 0
BOOL CALLBACK EnumWindowsProc(
	__in  HWND hwnd,
	__in  LPARAM lParam
	)
{
	ProcessRunningInfo * pInfo = (ProcessRunningInfo *)lParam;
	TCHAR exeFilePath[MAX_PATH] = {0};
	DWORD dwPID;
	::GetWindowThreadProcessId(hwnd, &dwPID);
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
	if (hProc == NULL)
	{
		return TRUE;
	}
	if (pInfo->bVistaOrLater)
	{
		if (NULL == pInfo->pQueryFullProcessImageName)
		{
			return FALSE;
		}
		DWORD dwSize = MAX_PATH;
		if ( !pInfo->pQueryFullProcessImageName(hProc, 0, exeFilePath, &dwSize) ||
			_tcsclen(exeFilePath) == 0 )
		{
			::CloseHandle(hProc);
			hProc = NULL;
			return TRUE;			
		}		
	}
	else
	{
		DWORD dwResult = ::GetModuleFileNameEx(hProc, NULL, exeFilePath, MAX_PATH);
		::CloseHandle(hProc);
		hProc = NULL;
		if (dwResult != 0 && _tcsclen(exeFilePath) == 0)
		{
			return TRUE;
		}
	}

	if(_tcsicmp(pInfo->szFilePath, exeFilePath)==0)
	{
		pInfo->bFound = TRUE;

		if (::IsIconic(hwnd))
		{
			::ShowWindow(hwnd, SW_RESTORE);
		}
		else
		{
			::BringWindowToTop(hwnd);
		}	

	}
	return TRUE;
}
#endif 

bool NaverExtensionBroker::StartUtility(const NPVariant *args, uint32_t argCount, NPVariant *result)
{

	//BOOL _retval = FALSE;
	//BOOLEAN_TO_NPVARIANT(_retval, *result);

	//uint32_t hkey = std::floor(NPVARIANT_TO_DOUBLE (args[0]) + 0.5);
	//NPString npPath = NPVARIANT_TO_STRING(args[1]);
	//NPString npItem = NPVARIANT_TO_STRING(args[2]);
	//NPString npFilename = NPVARIANT_TO_STRING(args[3]);
	//if( INTERNET_MAX_URL_LENGTH < npPath.UTF8Length || INTERNET_MAX_URL_LENGTH < npItem.UTF8Length || INTERNET_MAX_URL_LENGTH < npFilename.UTF8Length)
	//{
	//	return true;
	//}	
	//std::string path(npPath.UTF8Characters, npPath.UTF8Length);
	//std::string item(npItem.UTF8Characters, npItem.UTF8Length);
	//std::string filename(npFilename.UTF8Characters, npFilename.UTF8Length);
	//if (hkey == 0 || path.empty() || item.empty() || filename.empty())
	//	return false;

	//char pszBuffer[1024] = { 0 };
	//DWORD dwSize=1024;
	//DWORD dwType=0;
	//HKEY hKey;

	//DWORD flags = KEY_READ;
	//if (strstr(path.c_str(), "NaverVaccine") != 0)
	//	flags |= KEY_WOW64_64KEY;

	//if (RegOpenKeyExA((HKEY)hkey, path.c_str(),0,flags,&hKey) == ERROR_SUCCESS)
	//{
	//	if (ERROR_SUCCESS == RegQueryValueExA(hKey, item.c_str(),0,&dwType, (LPBYTE)pszBuffer,&dwSize) )
	//	{
	//		if (dwSize > 0)
	//		{
	//			pszBuffer[dwSize] = '\0';
	//			std::string filePath(pszBuffer);
	//			filePath += "\\";
	//			filePath += filename;

	//			OSVERSIONINFO osvi;

	//			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	//			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	//			GetVersionEx(&osvi);

	//			std::wstring wstrFileName = StringUtil::Utf8ToWString(filePath).c_str();
	//			ProcessRunningInfo info = {wstrFileName.c_str(), FALSE, FALSE, NULL};

	//			info.bVistaOrLater = (osvi.dwMajorVersion >= 6);


	//			HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	//			if (hModule)
	//			{
	//				info.pQueryFullProcessImageName = (PQueryFullProcessImageName)GetProcAddress(hModule, "QueryFullProcessImageNameW");
	//			}

	//			::EnumWindows(EnumWindowsProc, (LPARAM)(&info));

	//			if (hModule)
	//			{
	//				FreeLibrary(hModule);
	//			}

	//			if (info.bFound && (strstr(filePath.c_str(), "NaverCapture") == 0 && strstr(filePath.c_str(), "LINE") == 0))
	//			{
	//				// do nothing
	//			}
	//			else 
	//			{
	//				::ShellExecuteA(NULL, "open", filePath.c_str(), "", "", SW_SHOW );
	//			}
	//			_retval = TRUE;
	//		}
	//	}
	//	RegCloseKey(hKey);
	//	hKey = NULL;
	//}
	//BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}
bool NaverExtensionBroker::GetDefaultPicturePath(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	//std::wstring wPath= Util::GetMyPictures();
	//wPath.append(L"\\");
	//std::string sPath = StringUtil::WStringToUtf8(wPath);
	//char *pathBuffer = (char *) NPN_MemAlloc(MAX_PATH);
	//strncpy(pathBuffer, sPath.c_str(), MAX_PATH);
	//STRINGZ_TO_NPVARIANT(pathBuffer,*result);

	return TRUE;
}
bool NaverExtensionBroker::Is64BitOS(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL bIs64BitOS = FALSE;
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(_T("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(),&bIs64BitOS);
	}

	BOOLEAN_TO_NPVARIANT(bIs64BitOS, *result);
	return true;
}

bool NaverExtensionBroker::SelectDir(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = FALSE;

	BOOLEAN_TO_NPVARIANT(_retval, *result);

	BROWSEINFO bi;
	TCHAR displayBuffer[MAX_PATH] = { 0 };
	bi.hwndOwner = m_activeWindow;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = displayBuffer;
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if(pIDList)
	{
		char *pathBuffer = (char *) NPN_MemAlloc(MAX_PATH);
		SHGetPathFromIDListA(pIDList, pathBuffer);
		STRINGZ_TO_NPVARIANT(pathBuffer, *result);
		return true;
	}
	return false;
}

bool NaverExtensionBroker::SetCommandList()
{
	//HWND activeBrowser = GetActiveBrowser();

	//HWND hwnd = ::FindWindowExW(NULL, NULL, L"Chrome_WidgetWin_0" , NULL);
	//std::string className = WindowUtil::GetClassName(hwnd);
	//if (className.compare("Chrome_WidgetWin_0") != 0)
	//{
	//	Log::OutPut("<< chrome browser not found >>");
	//	return false;
	//}
	//if (hwnd)
	//{
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_l"),ApplicationConfig::Instance().GetGesture("arrow_l"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_r"),ApplicationConfig::Instance().GetGesture("arrow_r"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_u"),ApplicationConfig::Instance().GetGesture("arrow_u"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_d"),ApplicationConfig::Instance().GetGesture("arrow_d"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_su"),ApplicationConfig::Instance().GetGesture("arrow_su"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_sd"),ApplicationConfig::Instance().GetGesture("arrow_sd"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_ul"),ApplicationConfig::Instance().GetGesture("arrow_ul"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_ur"),ApplicationConfig::Instance().GetGesture("arrow_ur"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_dl"),ApplicationConfig::Instance().GetGesture("arrow_dl"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_dr"),ApplicationConfig::Instance().GetGesture("arrow_dr"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_lr"),ApplicationConfig::Instance().GetGesture("arrow_lr"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_rl"),ApplicationConfig::Instance().GetGesture("arrow_rl"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_du"),ApplicationConfig::Instance().GetGesture("arrow_du"));
	//	SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_ud"),ApplicationConfig::Instance().GetGesture("arrow_ud"));
	//	//SendMessage(hwnd,UM_SETCOMMAND,ChangCode("arrow_rd"),ApplicationConfig::Instance().GetGesture("arrow_rd"));

	//	std::string sColor = ApplicationConfig::Instance().GetString("gesture_color");
	//	SendMessage(hwnd,UM_SETGESTURELINE,1,m_MsgWnd.CovertRGB(sColor));
	//	SendMessage(hwnd,UM_SETGESTURELINE,2,ApplicationConfig::Instance().GetInt("gesture_lineSize"));
 //   
	//	m_indexNameFormart = ApplicationConfig::Instance().GetInt("capture_filenameFormat");
	//	m_PicFormart = ApplicationConfig::Instance().GetString("capture_saveFormat");
	//	m_sDefaultFilePath = ApplicationConfig::Instance().GetString("capture_savePath");

	//	return true;
	//}
	

	return false;
}
bool NaverExtensionBroker::SyncPreferences(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	//BOOL _retval = FALSE;
	//BOOLEAN_TO_NPVARIANT(_retval, *result);

	//NPString npPref = NPVARIANT_TO_STRING(args[0]);
	//if (INTERNET_MAX_URL_LENGTH < npPref.UTF8Length)
	//	return false;

	//std::string szPrefs(npPref.UTF8Characters, npPref.UTF8Length);
	//if (szPrefs.empty())
	//	return 0;

	//ApplicationConfig::Instance().Init(szPrefs, false);
	//SetCommandList();
	//if (ApplicationConfig::Instance().GetBool("gesture_isUsing", true))
	//	EnableGesture(true);
	//else
	//	EnableGesture(false);
	//
	//_retval = TRUE;
	//BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::onBeforeNavigate(const NPVariant *args, uint32_t argCount, NPVariant *result)
{

	BOOL _retval = TRUE;

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::onClickFun(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	bool bRet = NPN_InvokeDefault(mNpp,m_pJsCallBackObj,NULL,0,result);  
	return bRet;
}

void OnTestAsyncCall(void* pBroker)
{
	NPError err = NPERR_NO_ERROR;
	NaverExtensionBroker* pThis = (NaverExtensionBroker*)pBroker;
	NPAPI_VERIFY(NPN_GetURL(pThis->m_pNPInstance,"javascript:clickfun()","_self"));
}

unsigned int __stdcall NaverExtensionBroker::myTestThread(void* pParam)
{
	//MessageBox(NULL, TEXT("in myTestThread"), NULL, MB_OK);
	NPError err = NPERR_NO_ERROR;
	NaverExtensionBroker* pThis = (NaverExtensionBroker*)(pParam);

	NPN_PluginThreadAsyncCall(pThis->m_pNPInstance, OnTestAsyncCall, pThis);
	return 0;
}

bool NaverExtensionBroker::onTest(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	
	//NPError err = NPERR_NO_ERROR;
	//NPVariant	value;
	//NPAPI_VERIFY(NPN_GetValue(m_pNPInstance, NPNVdocumentOrigin, &value));

	//NPString log = NPVARIANT_TO_STRING(value);
	//std::string strInfo(log.UTF8Characters, log.UTF8Length);
	//NPN_ReleaseVariantValue(&value);

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, myTestThread, (void *)this, 0, NULL);
	if (hThread)
	{
		CloseHandle(hThread);
	}
	
	return true;
}

bool NaverExtensionBroker::onCompleted(const NPVariant *args, uint32_t argCount, NPVariant *result)
{

	BOOL _retval = TRUE;

	if ( NULL != args )
	{
		NPString url = NPVARIANT_TO_STRING(args[0]);
		if( INTERNET_MAX_URL_LENGTH < url.UTF8Length )
		{
			return true;
		}

		DWORD dwTime = GetTickCount();

		std::string szUrl(url.UTF8Characters, url.UTF8Length);
		//Log::OutPut( "----------------------------[onCompleted]-------------------------------" );
	}

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

bool NaverExtensionBroker::onCommitted(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	BOOL _retval = TRUE;

	if ( NULL != args && argCount == 4 )
	{
		int iTabId = std::floor(NPVARIANT_TO_DOUBLE (args[0]) + 0.5);
		NPString url = NPVARIANT_TO_STRING(args[1]);
		NPString transitionQualifiers = NPVARIANT_TO_STRING(args[2]);
		NPString  transitionType= NPVARIANT_TO_STRING (args[3]);
		DWORD dwTime = GetTickCount();
				
		std::string szUrl(url.UTF8Characters, url.UTF8Length);
		std::string sztransitionQualifiers(transitionQualifiers.UTF8Characters, transitionQualifiers.UTF8Length); 
		std::string szTransitionType(transitionType.UTF8Characters, transitionType.UTF8Length); 
		//Log::OutPut( "----------------------------[onCommitted]-------------------------------" );
		//Log::OutPut("[TIME : %d] TabId : %d,	url : %s",dwTime, iTabId, szUrl.c_str());
		//Log::OutPut("transitionType : %s,	transitionQualifiers : %s",szTransitionType.c_str(), sztransitionQualifiers.c_str());

		if( m_bAnableSendLog )
		{
			//m_logManager.AddLog( iTabId, szUrl.c_str(), sztransitionQualifiers.c_str(), szTransitionType.c_str());
		}
	}

	BOOLEAN_TO_NPVARIANT(_retval, *result);
	return true;
}

void NaverExtensionBroker::OnEventCall(LPCTSTR sContentsURL, int nType, int nErrorCode, LPCTSTR sMsg)
{
}

void NaverExtensionBroker::CallBackFunc(int itype,std::string &path)
{
	NPError err = NPERR_NO_ERROR;
	if (m_pJsCallBackObj)
	{

		 std::stringstream sBuf;
		 sBuf.clear();
		 sBuf<<"javascript:PluginCall("<<itype<<","<<"'"<<path<<"'"<<")";
		 
		 //Log::OutPut(sBuf.str());
		 NPAPI_VERIFY(NPN_GetURL(m_pNPInstance,sBuf.str().c_str(),"_self")); //vivid
	 }
}
int NaverExtensionBroker::ChangCode(char *buf)
{
	int code = 0;
	std::string strBuf(buf);
	if (strBuf.compare("arrow_l") == 0)
	{
		code =10;
	}
	else if(strBuf.compare("arrow_r") == 0)
	{
		code =20;
	}
	else if(strBuf.compare("arrow_u") == 0)
	{
		code =30;
	}
	else if(strBuf.compare("arrow_d") == 0)
	{
		code =40;
	}
	else if(strBuf.compare("arrow_su") == 0)
	{
		code =50;
	}
	else if(strBuf.compare("arrow_sd") == 0)
	{
		code =60;
	}
	else if(strBuf.compare("arrow_ul") == 0)
	{
		code =31;
	}
	else if(strBuf.compare("arrow_ur") == 0)
	{
		code =32;
	}
	else if(strBuf.compare("arrow_dl") == 0)
	{
		code =41;
	}
	else if(strBuf.compare("arrow_dr") == 0)
	{
		code =42;
	}
	else if(strBuf.compare("arrow_lr") == 0)
	{
		code =12;
	}
	else if(strBuf.compare("arrow_rl") == 0)
	{
		code =21;
	}
	else if(strBuf.compare("arrow_du") == 0)
	{
		code =43;
	}
	else if(strBuf.compare("arrow_ud") == 0)
	{
		code =34;
	}
	return code;
	
}
