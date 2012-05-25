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

#define __HTTP_VERB_GET			TEXT("GET")
#define __HTTP_VERB_POST		TEXT("POST")
#define __HTTP_ACCEPT_TYPE		TEXT("*/*")
#define __HTTP_ACCEPT			TEXT("Accept: */*\r\n")
#define __SIZE_HTTP_BUFFER	100000
#define __SIZE_HTTP_RESPONSE_BUFFER	100000
#define __SIZE_HTTP_HEAD_LINE	2048

#define __SIZE_BUFFER	1024
#define __SIZE_SMALL_BUFFER	256

#define __SZ_BOUNDARY			TEXT("--MULTI-PARTS-FORM-DATA-BOUNDARY")

class GenericHTTPClient
{
public:					
	struct GenericHTTPArgument
	{							// ARGUMENTS STRUCTURE
		ATL::CAtlString	szName;
		ATL::CAtlString  szValue;
		DWORD	dwType;

		GenericHTTPArgument()
		{
			szName = TEXT("");
			szValue = TEXT("");
			dwType = 0;
		}
		bool operator==(const GenericHTTPArgument &argV)
		{
			if (argV.dwType == dwType
				&& argV.szName == szName
				&& argV.szValue == szValue)
			{
				return true;
			}
			return false;
			//return !lstrcmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
		}
	};

	enum RequestMethod
	{
		// REQUEST METHOD
		RequestUnknown = 0,
		RequestGetMethod = 1,
		RequestPostMethod = 2,
		RequestPostMethodMultiPartsFormData = 3
	};

	enum TypePostArgument
	{
		// POST TYPE
		TypeUnknown = 0,
		TypeNormal = 1,
		TypeBinary = 2
	};

	// CONSTRUCTOR & DESTRUCTOR
	GenericHTTPClient();
	virtual ~GenericHTTPClient();

	//static GenericHTTPClient::RequestMethod GetMethod(int nMethod);
	//static GenericHTTPClient::TypePostArgument GetPostArgumentType(int nType);

	// Connection handler	
	BOOL Connect(LPCTSTR szAgent, 
		LPCTSTR szAddress, 
		WORD nPort = INTERNET_DEFAULT_HTTP_PORT, 
		LPCTSTR pszCookie = NULL,
		LPCTSTR szUserAccount = NULL, LPCTSTR szPassword = NULL);
	BOOL Close();

	VOID InitilizePostArguments();
	// HTTP Arguments handler	
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary = FALSE);

	VOID InitilizeRequestHeaders();
	VOID AddRequestHeader(LPCTSTR pszName);
	BOOL SendRequestHeaders();
	// HTTP Method handler 
	BOOL Request(LPCTSTR szURL, LPCTSTR szAgent,
		int nMethod = GenericHTTPClient::RequestGetMethod);
	BOOL RequestOfURI(LPCTSTR szURI, int nMethod = GenericHTTPClient::RequestGetMethod);
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
	std::vector<GenericHTTPArgument> _vArguments;				// POST ARGUMENTS VECTOR
	std::vector<CAtlString>			_vRequestHeaders;

	HINTERNET m_hOpen;				// internet open handle
	HINTERNET m_hConnection;		// internet connection hadle
	HINTERNET m_hRequest;		// internet request hadle

	TCHAR		m_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR
	TCHAR		m_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY

	DWORD		m_dwError;					// LAST ERROR CODE
	//LPCTSTR		m_szHost;					 //	 HOST NAME
	//DWORD		m_dwPort;					//  PORT
	DWORD		_dwResultSize;

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
