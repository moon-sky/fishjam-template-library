// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SETUPMAKERHELPER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SETUPMAKERHELPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SETUPMAKERHELPER_EXPORTS
#define SETUPMAKERHELPER_API __declspec(dllexport)
#else
#define SETUPMAKERHELPER_API __declspec(dllimport)
#endif

SETUPMAKERHELPER_API BOOL EnableSetupMonitor(DWORD curProcessId, DWORD dwSetupPID, HWND hWndGetSetupResult);
SETUPMAKERHELPER_API BOOL DisableSetupMonitor(DWORD dwSetupPID);

SETUPMAKERHELPER_API BOOL HookApi();
SETUPMAKERHELPER_API BOOL UnHookApi();


extern HHOOK g_hHookCallWndProc;
extern HHOOK g_hHookKeyboard;
extern HWND  g_hWndGetSetupResult;
extern DWORD g_dwSetupPID;

struct SETUP_MONITOR_INFO
{
    DWORD dwInfoType;   //1-file, 2-reg
    TCHAR szPath[MAX_PATH];
};