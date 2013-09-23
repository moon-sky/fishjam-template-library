

/*
  DESC: This file contains the code that implements an interface to the 
  DESC: Nullsoft NSIS Installer product script language. This file builds a
  DESC: dynamic link library and exports several methods for use in a script.
  DESC: Those methods are implemented in separate .c files in this project.
*/

#include "stdafx.h"
#include "nsisapi.h"


HINSTANCE g_hInstance;
HWND g_hwndParent;
int g_stringsize;
stack_t **g_stacktop;
TCHAR *g_variables;


BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
  g_hInstance=(HINSTANCE)hInst;
  return TRUE;
}


// utility functions (not required but often useful)
int popstring(TCHAR *str)
{
  stack_t *th;
  if (!g_stacktop || !*g_stacktop) return 1;
  th=(*g_stacktop);
  lstrcpy(str,th->text);
  *g_stacktop = th->next;
  GlobalFree((HGLOBAL)th);
  return 0;
}

void pushstring(TCHAR *str)
{
  stack_t *th;
  if (!g_stacktop) return;
  th=(stack_t*)GlobalAlloc(GPTR,sizeof(stack_t)+ g_stringsize * sizeof(TCHAR));
  lstrcpyn(th->text,str,g_stringsize);
  th->next=*g_stacktop;
  *g_stacktop=th;
}

TCHAR *getuservariable(int varnum)
{
  if (varnum < 0 || varnum >= __INST_LAST) return NULL;
  return g_variables+varnum*g_stringsize;
}
