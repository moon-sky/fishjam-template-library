// CFGenericHTTPClient.cpp: implementation of the CFGenericHTTPClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFGenericHTTPClient.h"
#include <winreg.h>
#include "../../Common/Conversion.h"

const TCHAR *HTTP_MIME[]    = { _T ( "*/*" ), _T ( "text/*" ), NULL } ;


//CFGenericHTTPClient::RequestMethod CFGenericHTTPClient::GetMethod(int nMethod)
//{	
//	return nMethod <=0 ? CFGenericHTTPClient::RequestGetMethod : 
//		static_cast<CFGenericHTTPClient::RequestMethod>(nMethod);
//}

//CFGenericHTTPClient::TypePostArgument CFGenericHTTPClient::GetPostArgumentType(int nType)
//{
//	return nType<=0 ? CFGenericHTTPClient::TypeNormal : static_cast<CFGenericHTTPClient::TypePostArgument>(nType);
//}


BOOL CFGenericHTTPClient::RequestOfURI(LPCTSTR szURI, int nMethod)
{

	BOOL bRet=TRUE;
	switch(nMethod)
	{
	case CFGenericHTTPClient::RequestPostMethod:		
		API_VERIFY(RequestPost(szURI));
		break;
	case CFGenericHTTPClient::RequestPostMethodMultiPartsFormData:
		API_VERIFY(RequestPostMultiPartsFormData(szURI));
		break;
	case CFGenericHTTPClient::RequestGetMethod:
	default:
		API_VERIFY(RequestGet(szURI));
		break;
	}

	return bRet;
}


BOOL CFGenericHTTPClient::RequestPost(LPCTSTR szURI)
{
	BOOL bRet = FALSE;

	CONST TCHAR *szAcceptType=__HTTP_ACCEPT_TYPE;
	TCHAR			szPostArguments[__SIZE_BUFFER]= { 0 } ;
	LPCTSTR szContentType = TEXT("Content-Type: application/x-www-form-urlencoded\r\n");

	DWORD dwPostArgumentsLegnth=GetPostArguments(szPostArguments, __SIZE_BUFFER);

	m_hRequest=::HttpOpenRequest(	m_hConnection,
		__HTTP_VERB_POST, // HTTP Verb
		szURI, // Object Name
		HTTP_VERSION, // Version
		TEXT(""), // Reference
										&HTTP_MIME[1], // Accept Type
		INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT,
		0); // context call-back point

	API_ASSERT(m_hRequest);

	if(!m_hRequest){
		m_dwError=::GetLastError();
		return FALSE;
	}

	// REPLACE HEADER
	API_VERIFY(::HttpAddRequestHeaders( m_hRequest, __HTTP_ACCEPT, _tcslen(__HTTP_ACCEPT), 
		HTTP_ADDREQ_FLAG_REPLACE));
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}


	// SEND REQUEST
	API_VERIFY(::HttpSendRequest( m_hRequest,	// handle by returned HttpOpenRequest
		szContentType, // additional HTTP header
							   _tcsclen ( szContentType ), // additional HTTP header length
		(LPVOID)szPostArguments, // additional data in HTTP Post or HTTP Put
							   _tcsclen ( szPostArguments ) ) // additional data length
		);
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	return TRUE;
}

BOOL CFGenericHTTPClient::RequestPostMultiPartsFormData(LPCTSTR szURI)
{
	BOOL bRet = FALSE;

	CONST TCHAR *szAcceptType=__HTTP_ACCEPT_TYPE;	
	LPCTSTR szContentType=TEXT("Content-Type: multipart/form-data; boundary=--MULTI-PARTS-FORM-DATA-BOUNDARY\r\n");

	// ALLOCATE POST MULTI-PARTS FORM DATA ARGUMENTS
	PBYTE pPostBuffer=NULL;
	DWORD dwPostBufferLength=AllocMultiPartsFormData(pPostBuffer, TEXT("--MULTI-PARTS-FORM-DATA-BOUNDARY"));

	m_hRequest=::HttpOpenRequest(	m_hConnection,
		__HTTP_VERB_POST, // HTTP Verb
		szURI, // Object Name
										NULL, //HTTP_VERSION, // Version
										NULL, //_T ( "" ), // Reference
										NULL, //&HTTP_MIME[1], // Accept Type
										INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, //INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT,	// flags
		0); // context call-back point

	API_ASSERT(m_hRequest);
	if(!m_hRequest){
		m_dwError=::GetLastError();
		return FALSE;
	}

	InitilizeRequestHeaders();
	AddRequestHeader(szContentType);
	AddRequestHeader(_T("Accept: */*"));

	CAtlString strTempRequest;
	strTempRequest.Format(_T("Content-Length: %d"), dwPostBufferLength);
	AddRequestHeader(strTempRequest);
	
	strTempRequest.Format(TEXT("User-Agent: %s"), __DEFAULT_AGENT_NAME);
	AddRequestHeader(strTempRequest);

	AddRequestHeader(_T("Connection: Keep-Alive"));
	AddRequestHeader(_T("Cache-Control: no-cache"));
	strTempRequest.Format(_T("Cookie: %s"), szCookie);
	AddRequestHeader(strTempRequest);

	if (!SendRequestHeaders() )
	{
		_dwError = ::GetLastError();
		return FALSE;
	}

#if 0
	// REPLACE HEADER
	API_VERIFY(::HttpAddRequestHeaders( m_hRequest, __HTTP_ACCEPT, 
		_tcslen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	API_VERIFY(::HttpAddRequestHeaders( m_hRequest, szContentType, 
		_tcslen(szContentType), HTTP_ADDREQ_FLAG_ADD_IF_NEW));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	TCHAR	szContentLength[__SIZE_BUFFER]= {0};
	::ZeroMemory(szContentLength, __SIZE_BUFFER);

	_stprintf( szContentLength, TEXT("Content-Length: %d\r\n"), dwPostBufferLength);

	API_VERIFY(::HttpAddRequestHeaders( m_hRequest, szContentLength, 
		_tcslen(szContentLength), HTTP_ADDREQ_FLAG_ADD_IF_NEW));
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}
#endif 

#if 0 //#ifdef	_DEBUG
	TCHAR	szHTTPRequest[__SIZE_HTTP_BUFFER]= { 0};
	DWORD	dwHTTPRequestLength=__SIZE_HTTP_BUFFER;

	API_VERIFY(::HttpQueryInfo(m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, 
		szHTTPRequest, &dwHTTPRequestLength, NULL));
	FTLTRACE(szHTTPRequest);
#endif

	// SEND REQUEST WITH HttpSendRequestEx and InternetWriteFile
	INTERNET_BUFFERS InternetBufferIn={0};
	InternetBufferIn.dwStructSize=sizeof(INTERNET_BUFFERS);
	InternetBufferIn.Next=NULL;	

	API_VERIFY(::HttpSendRequestEx(m_hRequest, &InternetBufferIn, NULL, HSR_INITIATE, 0));
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	DWORD dwSendCount = 0;
	while (TRUE)
	{
		int nBufferSize = 0;
		nBufferSize = min(1024 * 512, dwPostBufferLength - dwSendCount);
		if (nBufferSize == 0)
		{
			break;
		}

		DWORD dwOutPostBufferLength = 0;
		API_VERIFY(::InternetWriteFile ( _hHTTPRequest, pPostBuffer + dwSendCount, nBufferSize, &dwOutPostBufferLength ));
		dwSendCount += dwOutPostBufferLength;

	}

#if 0
	DWORD dwOutPostBufferLength=0;
	API_VERIFY(::InternetWriteFile(m_hRequest, pPostBuffer, dwPostBufferLength, &dwOutPostBufferLength));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}
#endif 

	API_VERIFY(::HttpEndRequest(m_hRequest, NULL, HSR_INITIATE, 0));
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	// FREE POST MULTI-PARTS FORM DATA ARGUMENTS
	FreeMultiPartsFormData(pPostBuffer);
	FTLTRACE(TEXT("Will Success Return from RequestPostMultiPartsFormData\n"));

	return TRUE;
}

DWORD CFGenericHTTPClient::ResponseOfBytes(PBYTE pBuffer, DWORD dwSize)
{
	BOOL bRet = FALSE;
	FTLTRACE(TEXT("Enter ResponseOfBytes\n"));
	static DWORD dwBytes=0;
	API_ASSERT(m_hRequest);
	if(!m_hRequest){
		m_dwError=::GetLastError();
		return 0;
	}

	::ZeroMemory(pBuffer, dwSize);
	API_VERIFY(::InternetReadFile(	m_hRequest,
		pBuffer,
		dwSize,
		&dwBytes));
	if (!bRet)
	{
			m_dwError=::GetLastError();
			return 0;
	}

	return dwBytes;
}

BOOL CFGenericHTTPClient::Response ( PWORD pHeaderBuffer,
								   DWORD dwHeaderBufferLength,
								   PWORD pBuffer,
								   DWORD dwBufferLength,
								   DWORD &dwResultSize )
{
	BOOL bRet = FALSE;
	BYTE pResponseBuffer[__SIZE_BUFFER]= {0};	
	DWORD dwNumOfBytesToRead=0;

	API_ASSERT(m_hRequest);
	if(!m_hRequest){
		m_dwError=::GetLastError();
		return FALSE;
	}

	::ZeroMemory(pBuffer, dwBufferLength);
	dwResultSize=0;
	while((dwNumOfBytesToRead=ResponseOfBytes(pResponseBuffer, __SIZE_BUFFER))!=NULL && dwResultSize<dwBufferLength){
		LPCSTR tempBuffer = ( LPCSTR ) pResponseBuffer;
		CConversion conversion;
		LPWSTR wStr = conversion.MBCS_TO_UTF16 ( tempBuffer );
		::CopyMemory ( pBuffer, wStr, dwNumOfBytesToRead * 2 );
		dwResultSize+=dwNumOfBytesToRead;
	}

	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	API_VERIFY(::HttpQueryInfo(m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pHeaderBuffer, 
		&dwHeaderBufferLength, NULL));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	return (dwResultSize? TRUE: FALSE);
}

VOID CFGenericHTTPClient::AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary)
{

	GenericHTTPArgument	arg;
	arg.szName = szName;
	arg.szValue = szValue;
	
	if(!bBinary)
		arg.dwType = CFGenericHTTPClient::TypeNormal;
	else
		arg.dwType = CFGenericHTTPClient::TypeBinary;	

	_vArguments.push_back(arg);

	return;
}

VOID CFGenericHTTPClient::AddPostArguments(LPCTSTR szName, DWORD nValue){

	GenericHTTPArgument	arg;
	arg.szName = szName;
	arg.szValue.Format(TEXT("%d"), nValue);
	arg.dwType = CFGenericHTTPClient::TypeNormal;

	_vArguments.push_back(arg);
	return;
}

DWORD CFGenericHTTPClient::GetPostArguments(LPTSTR szArguments, DWORD dwLength)
{
	::ZeroMemory(szArguments, dwLength);
	ATL::CAtlString	strTemp;

	std::vector<GenericHTTPArgument>::iterator itArg = _vArguments.begin();
	for(; itArg < _vArguments.end(); )
	{
		strTemp += itArg->szName + TEXT("=") + itArg->szValue;
		if((++itArg)<_vArguments.end()){
			strTemp += TEXT("&");
		}
	}
	
	StringCchCopy(szArguments, dwLength - 1, strTemp);
	szArguments[dwLength - 1] = TEXT('\0');

	return _tcslen(szArguments);
}


VOID CFGenericHTTPClient::InitilizePostArguments()
{
	_vArguments.clear();
	return;
}

VOID CFGenericHTTPClient::InitilizeRequestHeaders()
{
	_vRequestHeaders.clear();
}

VOID CFGenericHTTPClient::AddRequestHeader(LPCTSTR pszName)
{
	_vRequestHeaders.push_back(pszName);
}

BOOL CFGenericHTTPClient::SendRequestHeaders()
{
	BOOL bRet = FALSE;
	std::vector<CAtlString>::iterator iter = _vRequestHeaders.begin();
	for (; iter != _vRequestHeaders.end(); ++iter)
	{
		CAtlString strHeader = *iter + _T("\r\n");
		FTLTRACE(TEXT("SendRequestHeaders >>>>  %s\n"), strHeader);

		bRet = ::HttpAddRequestHeaders(_hHTTPRequest, (LPTSTR)(LPCTSTR)strHeader, 
			strHeader.GetLength(), HTTP_ADDREQ_FLAG_ADD_IF_NEW);
		API_ASSERT(bRet);
	}
	return bRet;
}

VOID CFGenericHTTPClient::FreeMultiPartsFormData(PBYTE &pBuffer)
{

	if(pBuffer)
	{
		::LocalFree(pBuffer);
		pBuffer=NULL;
	}

	return;
}

DWORD CFGenericHTTPClient::AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary)
{
	if(pInBuffer)
	{
		::LocalFree(pInBuffer);
		pInBuffer=NULL;
	}

	pInBuffer=(PBYTE)::LocalAlloc(LPTR, GetMultiPartsFormDataLength());	
	std::vector<GenericHTTPArgument>::iterator itArgv;

	DWORD dwPosition=0;
	DWORD dwBufferSize=0;

	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv)
	{

		PBYTE pBuffer=NULL;

		// SET MULTI_PRATS FORM DATA BUFFER
		switch(itArgv->dwType)
		{
		case	CFGenericHTTPClient::TypeNormal:
			pBuffer=(PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE*4);
				StringCchPrintfA(( char* ) pBuffer, __SIZE_HTTP_HEAD_LINE * 4,
								 "--%s\r\n"
								 "Content-Disposition: form-data; name=\"%s\"\r\n"
								 "\r\n"
								 "%s\r\n",
								 ConvertWCtoC(szBoundary),
								 ConvertWCtoC(itArgv->szName),
								 ConvertWCtoC(itArgv->szValue) );

				dwBufferSize = strlen ( ( char* ) pBuffer );

			break;
		case	CFGenericHTTPClient::TypeBinary:
			DWORD	dwNumOfBytesToRead=0;
			DWORD	dwTotalBytes=0;

			HANDLE hFile=::CreateFile(itArgv->szValue, 
				GENERIC_READ, // desired access
				FILE_SHARE_READ, // share mode
				NULL, // security attribute
				OPEN_EXISTING, // create disposition
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, // flag and attributes
				NULL); // template file handle

			DWORD	dwSize=::GetFileSize(hFile, NULL);

			pBuffer=(PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE*3+dwSize+1);
			BYTE	pBytes[__SIZE_BUFFER+1]="";
				StringCchPrintfA ( ( char* ) pBuffer, __SIZE_HTTP_HEAD_LINE * 4,
								   "--%s\r\n"
								   "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
								   "Content-Type: %s\r\n"
								   "\r\n",
								   ConvertWCtoC ( szBoundary ),
								   ConvertWCtoC ( itArgv->szName ), ConvertWCtoC ( itArgv->szValue ),
								   "image/jpeg"
								   //ConvertWCtoC ( GetContentType ( itArgv->szValue ) )
								 );

				DWORD	dwBufPosition = strlen ( ( char* ) pBuffer );

			while(::ReadFile(hFile, pBytes, __SIZE_BUFFER, &dwNumOfBytesToRead, NULL) && dwNumOfBytesToRead>0 && dwTotalBytes<=dwSize)
			{
				::CopyMemory((pBuffer+dwBufPosition+dwTotalBytes), pBytes, dwNumOfBytesToRead);
				::ZeroMemory(pBytes, __SIZE_BUFFER+1);
				dwTotalBytes+=dwNumOfBytesToRead;				
			}

				::CopyMemory ( ( pBuffer + dwBufPosition + dwTotalBytes ), "\r\n", strlen ( "\r\n" ) * sizeof ( char ) );
				::CloseHandle ( hFile );

				dwBufferSize = dwBufPosition + dwTotalBytes + strlen ( "\r\n" ) * sizeof ( char );
				break;
		}

		::CopyMemory((pInBuffer+dwPosition), pBuffer, dwBufferSize);
		dwPosition+=dwBufferSize;

		if(pBuffer)
		{
			::LocalFree(pBuffer);
			pBuffer=NULL;
		}
	}

	::CopyMemory ( ( pInBuffer + dwPosition ), "--", 2 * sizeof ( char ) );
	::CopyMemory ( ( pInBuffer + dwPosition + ( 2 * sizeof ( char ) ) ), ConvertWCtoC ( szBoundary ), strlen ( ConvertWCtoC ( szBoundary ) ) * sizeof ( char ) );
	::CopyMemory ( ( pInBuffer + dwPosition + ( 2 + strlen ( ConvertWCtoC ( szBoundary ) ) ) * sizeof ( char ) ), "--\r\n", 3 * sizeof ( char ) );

	return dwPosition + ( ( 2 + strlen ( ConvertWCtoC ( szBoundary ) ) + 3 ) * sizeof ( char ) );
}

DWORD CFGenericHTTPClient::GetMultiPartsFormDataLength()
{
	std::vector<GenericHTTPArgument>::iterator itArgv;

	DWORD	dwLength=0;

	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv)
	{

		switch(itArgv->dwType)
		{
		case	CFGenericHTTPClient::TypeNormal:
			dwLength+=__SIZE_HTTP_HEAD_LINE*4;
			break;
		case	CFGenericHTTPClient::TypeBinary:
			HANDLE hFile=::CreateFile(itArgv->szValue, 
				GENERIC_READ, // desired access
				FILE_SHARE_READ, // share mode
				NULL, // security attribute
				OPEN_EXISTING, // create disposition
				FILE_ATTRIBUTE_NORMAL, // flag and attributes
				NULL); // template file handle

			dwLength+=__SIZE_HTTP_HEAD_LINE*3+::GetFileSize(hFile, NULL);
			::CloseHandle(hFile);			
			break;
		}

	}

	return dwLength;
}

LPCTSTR CFGenericHTTPClient::GetContentType(LPCTSTR szName)
{
	static TCHAR	szReturn[1024]= { 0 };
	LONG	dwLen=1024;
	DWORD	dwDot=0;

	for ( dwDot = _tcsclen ( szName ); dwDot > 0; dwDot-- )
	{
		if(!_tcsncmp((szName+dwDot),TEXT("."), 1))
		{
			break;
		}
	}

	HKEY	hKEY;
	LPTSTR	szWord=(TCHAR*)(szName+dwDot);

	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, szWord, 0, KEY_QUERY_VALUE, &hKEY)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKEY, TEXT("Content Type"), NULL, NULL, (LPBYTE)szReturn, 
			(unsigned long*)&dwLen) != ERROR_SUCCESS)
		{
			_tcsncpy(szReturn, TEXT("application/octet-stream"), _tcslen(TEXT("application/octet-stream")));
		}
		RegCloseKey(hKEY);
	}
	else
	{
		_tcsncpy ( szReturn, _T ( "application/octet-stream" ), _tcsclen ( _T ( "application/octet-stream" ) ) );
	}

	return szReturn;
}

DWORD CFGenericHTTPClient::GetLastError()
{
	return m_dwError;
}

VOID CFGenericHTTPClient::ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, WORD &wPort, LPTSTR szURI)
{

	TCHAR szPort[__SIZE_BUFFER]= { 0 };
	DWORD dwPosition=0;
	BOOL bFlag=FALSE;

	while ( _tcsclen ( szURL ) > 0 && dwPosition < _tcsclen ( szURL ) && _tcsncmp ( ( szURL + dwPosition ), _T ( ":" ), 1 ) )
	{
		++dwPosition;
	}

	if(!_tcsncmp((szURL+dwPosition+1), TEXT("/"), 1))
	{	// is PROTOCOL
		if(szProtocol)
		{
			_tcsncpy(szProtocol, szURL, dwPosition);
			szProtocol[dwPosition]=0;
		}
		bFlag=TRUE;
	}
	else
	{	// is HOST 
		if(szProtocol){
			_tcsncpy(szProtocol, TEXT("http"), 4);
			szProtocol[5]=0;
		}
	}

	DWORD dwStartPosition=0;

	if(bFlag){
		dwStartPosition=dwPosition+=3;				
	}else{
		dwStartPosition=dwPosition=0;
	}

	bFlag=FALSE;
	while(_tcslen(szURL)>0 && dwPosition<_tcsclen(szURL) && _tcsncmp((szURL+dwPosition), TEXT("/"), 1))
		++dwPosition;

	DWORD dwFind=dwStartPosition;

	for(;dwFind<=dwPosition;dwFind++)
	{
		if(!_tcsncmp((szURL+dwFind), TEXT(":"), 1))
		{ // find PORT
			bFlag=TRUE;
			break;
		}
	}

	if(bFlag)
	{
		TCHAR sztmp[__SIZE_BUFFER]= { 0 };
		_tcsncpy(sztmp, (szURL+dwFind+1), dwPosition-dwFind);
		wPort=_ttol(sztmp);
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwFind-dwStartPosition);
	}else if(!lstrcmp(szProtocol,TEXT("https"))){
		wPort=INTERNET_DEFAULT_HTTPS_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}else {
		wPort=INTERNET_DEFAULT_HTTP_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}

	if(dwPosition<_tcsclen(szURL))
	{ // find URI
		_tcsncpy(szURI, (szURL+dwPosition), _tcslen(szURL)-dwPosition);
	}else{
		szURI[0]=0;
	}

	return;
}

LPCTSTR CFGenericHTTPClient::QueryHTTPResponseHeader()
{
	return m_szHTTPResponseHeader;
}

LPCTSTR CFGenericHTTPClient::QueryHTTPResponse()
{
	return m_szHTTPResponseHTML;
}
DWORD CFGenericHTTPClient::QueryHTTPResponseSize()
{
	return m_dwResultSize;
}

LPCSTR CFGenericHTTPClient::ConvertWCtoC ( LPCTSTR str )
{
	char* pStr ;
	int strSize = WideCharToMultiByte ( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
	pStr = new char[strSize];
	WideCharToMultiByte ( CP_ACP, 0, str, -1, pStr, strSize, 0, 0 );
	return pStr;
}
