
#ifndef FTL_WIN_INTERFACE_DETECT_H
#define FTL_WIN_INTERFACE_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWinIntDetect.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlWinIntDetect.h requires ftlCom.h to be included first
#endif


/*********************************************************************************************************************************

*********************************************************************************************************************************/

namespace FTL
{

	class CFWinInterfaceDetectUtil
	{
	public:
		FTLINLINE static LPCTSTR GetSystemStateFlags(CFStringFormater& formater, DWORD nFlags, LPCTSTR pszDivide = TEXT("|"));
	};

	class CFEnumVARIANTDumper: public CFInterfaceDumperBase<CFEnumVARIANTDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFEnumVARIANTDumper);
	public:
		FTLINLINE explicit CFEnumVARIANTDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFEnumVARIANTDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	/*********************************************************************************************************************************
	* IAccessible -- MSAA(Microsoft Active Accessibility), 活动的可访问兼容性应用。可通过微软的 Inspect.exe 工具查看
	*   UI程序暴露出一个Interface，方便另一个程序对其进行控制，初衷是为了方便残疾人，可用于对象抓图、UI自动化测试等。
	*   AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, IID_IAccessible, (void**)&paccMainWindow)，
	*     对应窗体会接收到 WM_GETOBJECT 消息，创建实现了IAccessible的内部类(MS在.NET WinForm Framework 中提供了IAccessible的默认实现,
	*     其他的有 Local oleacc proxy?)，然后通过 LresultFromObject 返回该接口
	*********************************************************************************************************************************/

	class CFAccessibleDumper : public CFInterfaceDumperBase<CFAccessibleDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFAccessibleDumper);
	public:
		FTLINLINE explicit CFAccessibleDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFAccessibleDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

}

#endif //FTL_WIN_INTERFACE_DETECT_H



#ifndef USE_EXPORT
#  include "ftlWinIntDetect.hpp"
#endif
