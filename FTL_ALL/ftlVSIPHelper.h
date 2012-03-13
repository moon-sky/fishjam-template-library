#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlVSIPHelper.h requires ftlCom.h to be included first
#endif

/********************************************************************************************

********************************************************************************************/

namespace FTL
{
    class CFVSIPUtils
    {
    public:

        //获取 cmdidcmd.h 中定义的 CmdID 对应的字符串，在 IOleCommandTarget::QueryStatus 中判断 cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);
    };



	class CFVsPackageDumper : public CFInterfaceDumperBase<CFVsPackageDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsPackageDumper);
	public:
		FTLINLINE explicit CFVsPackageDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsPackageDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextManagerDumper : public CFInterfaceDumperBase<CFVsTextManagerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextManagerDumper);
	public:
		FTLINLINE explicit CFVsTextManagerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextManagerDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextMarkerTypeDumper : public CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextMarkerTypeDumper);
	public:
		FTLINLINE explicit CFVsTextMarkerTypeDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};


	//Window
	class CFVsTextViewDumper : public CFInterfaceDumperBase<CFVsTextViewDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextViewDumper);
	public:
		FTLINLINE explicit CFVsTextViewDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextViewDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
