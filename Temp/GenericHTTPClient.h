#ifndef __GENERIC_HTTP_CLIENT
#define __GENERIC_HTTP_CLIENT

#include <tchar.h>
#include <wininet.h>
#include <atlstr.h>

// PRE-DEFINED CONSTANTS
//#define __DEFAULT_AGENT_NAME	"MERONG(0.9/;p)"

// PRE-DEFINED BUFFER SIZE
#define	__SIZE_HTTP_ARGUMENT_NAME	256
#define __SIZE_HTTP_ARGUMENT_VALUE	1024

#define __SIZE_HTTP_BUFFER	100000
#define __SIZE_HTTP_RESPONSE_BUFFER	100000
#define __SIZE_HTTP_HEAD_LINE	2048

#define __SIZE_SMALL_BUFFER	256

#define __SZ_BOUNDARY			TEXT("--MULTI-PARTS-FORM-DATA-BOUNDARY")

class CFGenericHTTPClient
{
public:					
	// CONSTRUCTOR & DESTRUCTOR

	//static CFGenericHTTPClient::RequestMethod GetMethod(int nMethod);
	//static CFGenericHTTPClient::TypePostArgument GetPostArgumentType(int nType);

	// Connection handler	

	VOID InitilizePostArguments();
	// HTTP Arguments handler	
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary = FALSE);

	VOID InitilizeRequestHeaders();
	VOID AddRequestHeader(LPCTSTR pszName);
	BOOL SendRequestHeaders();
	BOOL RequestOfURI(LPCTSTR szURI, int nMethod = CFGenericHTTPClient::RequestGetMethod);
	BOOL Response(PWORD pHeaderBuffer, DWORD dwHeaderBufferLength, PWORD pBuffer, DWORD dwBufferLength, DWORD &dwResultSize);

	LPCTSTR QueryHTTPResponse();
	DWORD	QueryHTTPResponseSize();
	LPCTSTR QueryHTTPResponseHeader();	

	// General Handler
	DWORD GetLastError();
	LPCTSTR GetContentType(LPCTSTR szName);
	VOID ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, WORD &wPort, LPTSTR szURI);
	
	LPCSTR ConvertWCtoC(LPCTSTR str);
protected:				
	//LPCTSTR		m_szHost;					 //	 HOST NAME
	//DWORD		m_dwPort;					//  PORT
	DWORD		m_dwResultSize;

	// HTTP Method handler
	DWORD ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	DWORD GetPostArguments(LPTSTR szArguments, DWORD dwLength);
	BOOL RequestPost(LPCTSTR szURI);
	BOOL RequestPostMultiPartsFormData(LPCTSTR szURI);
	BOOL RequestGet(LPCTSTR szURI);
	DWORD AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary);
	VOID FreeMultiPartsFormData(PBYTE &pBuffer);
	DWORD GetMultiPartsFormDataLength();
};

#endif	// #ifndef __GENERIC_HTTP_CLIENT
