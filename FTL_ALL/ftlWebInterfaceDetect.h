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

*********************************************************************************************************************************/

namespace FTL
{
	/*********************************************************************************************************************************
	* IWebBrowser
	*********************************************************************************************************************************/

	class CFWebBrowserDumper : public CFInterfaceDumperBase<CFWebBrowserDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserDumper);
	public:
		FTLINLINE explicit CFWebBrowserDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowserDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//IHTMLDocument6
	//IHTMLDocument7
	class CFHTMLDocumentDumper : public CFInterfaceDumperBase<CFHTMLDocumentDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLDocumentDumper);
	public:
		FTLINLINE explicit CFHTMLDocumentDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFHTMLDocumentDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLStyleDumper : public CFInterfaceDumperBase<CFHTMLStyleDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLStyleDumper);
	public:
		FTLINLINE explicit CFHTMLStyleDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFHTMLStyleDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLElementDumper : public CFInterfaceDumperBase<CFHTMLElementDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementDumper);
	public:
		FTLINLINE explicit CFHTMLElementDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFHTMLElementDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLElementCollectionDumper : public CFInterfaceDumperBase<CFHTMLElementCollectionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementCollectionDumper);
	public:
		FTLINLINE explicit CFHTMLElementCollectionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFHTMLElementCollectionDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
}

#endif //FTL_WEB_INTERFACE_DETECT_H



#ifndef USE_EXPORT
#  include "ftlWebInterfaceDetect.hpp"
#endif
