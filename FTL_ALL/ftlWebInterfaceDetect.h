#ifndef FTL_WEB_INTERFACE_DETECT_H
#define FTL_WEB_INTERFACE_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWebInterfaceDetect.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlWebInterfaceDetect.h requires ftlCom.h to be included first
#endif


/*********************************************************************************************************************************
* 在 WebBrowser(IWebBrowser) 中屏蔽脚本错误 -- http://www.cnblogs.com/zhangqingping/archive/2009/06/16/1504260.html
*   方法1: SetSilent -- 其它提示信息(如 alert)也都不显示
*   方法2: 重载 IOleCommandTarget::Exec，过滤 nCmdID == OLECMDID_SHOWSCRIPTERROR 的情况;
*   方法3: 在 OnNavigateComplete2 或 OnNavigateComplete 中设置 JavaScript 的 window.onerror 事件
*          测试时发现 OnNavigateComplete2 时无法QI到 IHTMLDocument2
*   方法4: this.webBrowser1.ScriptErrorsSuppressed = true;
*********************************************************************************************************************************/

namespace FTL
{
	/*********************************************************************************************************************************
	* IWebBrowser
	*********************************************************************************************************************************/


	enum FindElementMethod
	{
		femById,
		femByTag,
		femByClass,
	};
	class CFWebBrowserUtil
	{
	public:
		FTLINLINE static HRESULT FindChildElement(IHTMLElement* pParent, 
			FindElementMethod method, LPCTSTR pszName, LONG nStartIndex, RecursiveWay recurWay,
			IHTMLElement** ppFoundElement);
	};

	class CFWebBrowserDumper : public CFInterfaceDumperBase<CFWebBrowserDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserDumper);
	public:
		FTLINLINE explicit CFWebBrowserDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFWebBrowserDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//IHTMLDocument6
	//IHTMLDocument7
	class CFHTMLDocumentDumper : public CFInterfaceDumperBase<CFHTMLDocumentDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLDocumentDumper);
	public:
		FTLINLINE explicit CFHTMLDocumentDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLDocumentDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//可以控制 focus 等状态
	class CFHTMLWindowDumper: public CFInterfaceDumperBase<CFHTMLWindowDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLWindowDumper);
	public:
		FTLINLINE explicit CFHTMLWindowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLWindowDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//可以通过 IHTMLWindow2::get_event 获取到 IHTMLEventObj，然后通过 IHTMLEventObj::get_srcElement 获取激发事件的元素(如点击的按钮等)
	class CFHTMLEventObjDumper : public CFInterfaceDumperBase<CFHTMLEventObjDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLEventObjDumper);
	public:
		FTLINLINE explicit CFHTMLEventObjDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLEventObjDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLStyleDumper : public CFInterfaceDumperBase<CFHTMLStyleDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLStyleDumper);
	public:
		FTLINLINE explicit CFHTMLStyleDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLStyleDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//param参数是可以是在Collection中的索引，通常由 CFHTMLElementCollectionDumper 调用时设置
	class CFHTMLElementDumper : public CFInterfaceDumperBase<CFHTMLElementDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementDumper);
	public:
		FTLINLINE explicit CFHTMLElementDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLElementDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLElementCollectionDumper : public CFInterfaceDumperBase<CFHTMLElementCollectionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementCollectionDumper);
	public:
		FTLINLINE explicit CFHTMLElementCollectionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLElementCollectionDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLTableDumper : public CFInterfaceDumperBase<CFHTMLTableDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLTableDumper);
	public:
		FTLINLINE explicit CFHTMLTableDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLTableDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLTableRowDumper : public CFInterfaceDumperBase<CFHTMLTableRowDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLTableRowDumper);
	public:
		FTLINLINE explicit CFHTMLTableRowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLTableRowDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
}

#endif //FTL_WEB_INTERFACE_DETECT_H



#ifndef USE_EXPORT
#  include "ftlWebInterfaceDetect.hpp"
#endif
