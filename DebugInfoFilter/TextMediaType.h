#pragma once

//可以通过 http://mikolajapp.appspot.com/uuid/ 网站查询各种 GUID

class CTextMediaType : public CMediaType {

public:

    CTextMediaType(AM_MEDIA_TYPE mt):CMediaType(mt) {}

    HRESULT AsText(LPTSTR szType,unsigned int iLen);
	HRESULT AsText(FTL::CFStringFormater& strFormater);
	static HRESULT CLSID2String(LPTSTR szBuffer, UINT iLength, const GUID* pGuid);
    
    //返回关联指定CLSID的COM组件的位置
    //这两个方法在传递的GUID是Filter的时是正确的 -- 从 SysEnum 中拷贝而来，尚未确认其他的
    static HRESULT GetFilenameByCLSIDString(const WCHAR *szGUID, WCHAR *szFile, size_t cch);
    static HRESULT GetFilenameByCLSID(const GUID *pGUID, WCHAR *szFile, size_t cch);
private:

    // Provide a string description for this format block

    HRESULT Format2String(LPTSTR szBuffer,
                       UINT iLength,
                       const GUID* pGuid,
                       BYTE* pFormat,
                       ULONG lFormatLength);

    // Convert this CLSID into a meaningful string


	HRESULT _GetVideoInfoHeaderInfo(LPTSTR szBuffer, UINT iLength, 
		LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength);

	HRESULT _GetWaveFormatInfo(LPTSTR szBuffer, UINT iLength, 
		LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength);

};

