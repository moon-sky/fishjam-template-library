// GenericHTTPClient.cpp: implementation of the GenericHTTPClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GenericHTTPClient.h"
#include <winreg.h>
#pragma comment(lib , "Wininet.lib")


GenericHTTPClient::GenericHTTPClient()
{
	m_hOpen = NULL;
	m_hConnection = NULL;
	m_hRequest=NULL;
	
	ZeroMemory(m_szHTTPResponseHeader, sizeof(m_szHTTPResponseHeader));
	ZeroMemory(m_szHTTPResponseHTML, sizeof(m_szHTTPResponseHTML));

	m_dwError = 0;
	//m_szHost = NULL;
	//m_dwPort = 0;
}

GenericHTTPClient::~GenericHTTPClient()
{
	Close();
}

//GenericHTTPClient::RequestMethod GenericHTTPClient::GetMethod(int nMethod)
//{	
//	return nMethod <=0 ? GenericHTTPClient::RequestGetMethod : 
//		static_cast<GenericHTTPClient::RequestMethod>(nMethod);
//}

//GenericHTTPClient::TypePostArgument GenericHTTPClient::GetPostArgumentType(int nType)
//{
//	return nType<=0 ? GenericHTTPClient::TypeNormal : static_cast<GenericHTTPClient::TypePostArgument>(nType);
//}

BOOL GenericHTTPClient::Connect(LPCTSTR szAgent,
								LPCTSTR szAddress, 
								WORD nPort /* = INTERNET_DEFAULT_HTTP_PORT */, 
								LPCTSTR pszCookie /* = NULL */, 
								LPCTSTR szUserAccount /* = NULL */, 
								LPCTSTR szPassword /* = NULL */)
{
	BOOL bRet = FALSE;

	m_hOpen =::InternetOpen(szAgent,	// agent name
		INTERNET_OPEN_TYPE_PRECONFIG,	// proxy option
		NULL,   // proxy
		NULL,	// proxy bypass
		0);		// flags

	API_ASSERT(m_hOpen);

	if(!m_hOpen)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}	

	m_hConnection=::InternetConnect(	m_hOpen,	// internet opened handle
		szAddress, // server name
		nPort, // ports
		szUserAccount, // user name
		szPassword, // password 
		INTERNET_SERVICE_HTTP, // service type
		INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,	// service option																														
		0);	// context call-back option

	if(!m_hConnection){		
		m_dwError=::GetLastError();
		SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);
		return FALSE;
	}

	if(::InternetAttemptConnect(NULL)!=ERROR_SUCCESS){		
		m_dwError=::GetLastError();
		SAFE_CLOSE_INTERNET_HANDLE(m_hConnection);
		SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);
		return FALSE;
	}

	return TRUE;															
}

BOOL GenericHTTPClient::Close()
{
	BOOL bRet = FALSE;
	SAFE_CLOSE_INTERNET_HANDLE(m_hRequest);
	SAFE_CLOSE_INTERNET_HANDLE(m_hConnection);
	SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);

	return bRet;
}

BOOL GenericHTTPClient::Request(LPCTSTR szURL, 
								LPCTSTR szAgent, 
								int nMethod)
{

	BOOL bReturn=TRUE;
	WORD wPort=0;
	TCHAR szProtocol[__SIZE_BUFFER]= { 0 };
	TCHAR szAddress[__SIZE_BUFFER]= { 0 };	
	TCHAR szURI[__SIZE_BUFFER]= {0};
	DWORD dwSize=0;

	ParseURL(szURL, szProtocol, szAddress, wPort, szURI);

	if(Connect(szAddress, szAgent, wPort))
	{
		if(!RequestOfURI(szURI, nMethod))
		{
			bReturn=FALSE;
		}
		else
		{
			if(!Response((PBYTE)m_szHTTPResponseHeader, __SIZE_HTTP_BUFFER, 
				(PBYTE)m_szHTTPResponseHTML, __SIZE_HTTP_BUFFER,
				dwSize))
			{
				bReturn=FALSE;		
			}
		}
		Close();
	}else{
		bReturn=FALSE;
	}

	return bReturn;
}

BOOL GenericHTTPClient::RequestOfURI(LPCTSTR szURI, int nMethod)
{

	BOOL bRet=TRUE;
	switch(nMethod)
	{
	case GenericHTTPClient::RequestPostMethod:		
		API_VERIFY(RequestPost(szURI));
		break;
	case GenericHTTPClient::RequestPostMethodMultiPartsFormData:
		API_VERIFY(RequestPostMultiPartsFormData(szURI));
		break;
	case GenericHTTPClient::RequestGetMethod:
	default:
		API_VERIFY(RequestGet(szURI));
		break;
	}

	return bRet;
}

BOOL GenericHTTPClient::RequestGet(LPCTSTR szURI)
{
	BOOL bRet = FALSE;

	CONST TCHAR *szAcceptType=__HTTP_ACCEPT_TYPE;

	m_hRequest =::HttpOpenRequest(	m_hConnection,
		__HTTP_VERB_GET, // HTTP Verb
		szURI, // Object Name
		HTTP_VERSION, // Version
		TEXT(""), // Reference
		NULL, //&szAcceptType, // Accept Type
		INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
		0); // context call-back point

	API_ASSERT(!!m_hRequest);
	if(!m_hRequest)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	// REPLACE HEADER
	API_VERIFY(::HttpAddRequestHeaders( m_hRequest, __HTTP_ACCEPT, 
		_tcslen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_ADD_IF_NEW));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	// SEND REQUEST
	API_VERIFY(::HttpSendRequest( m_hRequest,	// handle by returned HttpOpenRequest
		NULL, // additional HTTP header
		0, // additional HTTP header length
		NULL, // additional data in HTTP Post or HTTP Put
		0)); // additional data length
	//if(!bRet)
	//{
	//	m_dwError=::GetLastError();
	//	return FALSE;
	//}

	return TRUE;
}

BOOL GenericHTTPClient::RequestPost(LPCTSTR szURI)
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
		&szAcceptType, // Accept Type
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
		_tcslen(szContentType), // additional HTTP header length
		(LPVOID)szPostArguments, // additional data in HTTP Post or HTTP Put
		_tcslen(szPostArguments)) // additional data length
		);
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	return TRUE;
}

BOOL GenericHTTPClient::RequestPostMultiPartsFormData(LPCTSTR szURI)
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
		HTTP_VERSION, // Version
		TEXT(""), // Reference
		&szAcceptType, // Accept Type
		INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT,	// flags
		0); // context call-back point

	API_ASSERT(m_hRequest);
	if(!m_hRequest){
		m_dwError=::GetLastError();
		return FALSE;
	}

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

#ifdef	_DEBUG
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

	DWORD dwOutPostBufferLength=0;
	API_VERIFY(::InternetWriteFile(m_hRequest, pPostBuffer, dwPostBufferLength, &dwOutPostBufferLength));
	if (!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	API_VERIFY(::HttpEndRequest(m_hRequest, NULL, HSR_INITIATE, 0));
	if(!bRet)
	{
		m_dwError=::GetLastError();
		return FALSE;
	}

	// FREE POST MULTI-PARTS FORM DATA ARGUMENTS
	FreeMultiPartsFormData(pPostBuffer);

	return TRUE;
}

DWORD GenericHTTPClient::ResponseOfBytes(PBYTE pBuffer, DWORD dwSize)
{
	BOOL bRet = FALSE;
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

BOOL GenericHTTPClient::Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, 
								 PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize)
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
		::CopyMemory( (pBuffer+dwResultSize), pResponseBuffer, dwNumOfBytesToRead);		
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

VOID GenericHTTPClient::AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary)
{

	GenericHTTPArgument	arg;
	arg.szName = szName;
	arg.szValue = szValue;
	
	if(!bBinary)
		arg.dwType = GenericHTTPClient::TypeNormal;
	else
		arg.dwType = GenericHTTPClient::TypeBinary;	

	_vArguments.push_back(arg);

	return;
}

VOID GenericHTTPClient::AddPostArguments(LPCTSTR szName, DWORD nValue){

	GenericHTTPArgument	arg;
	arg.szName = szName;
	arg.szValue.Format(TEXT("%d"), nValue);
	arg.dwType = GenericHTTPClient::TypeNormal;

	_vArguments.push_back(arg);
	return;
}

DWORD GenericHTTPClient::GetPostArguments(LPTSTR szArguments, DWORD dwLength)
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


VOID GenericHTTPClient::InitilizePostArguments()
{
	_vArguments.clear();
	return;
}

VOID GenericHTTPClient::FreeMultiPartsFormData(PBYTE &pBuffer)
{

	if(pBuffer)
	{
		::LocalFree(pBuffer);
		pBuffer=NULL;
	}

	return;
}

DWORD GenericHTTPClient::AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary)
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

	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv){

		PBYTE pBuffer=NULL;

		// SET MULTI_PRATS FORM DATA BUFFER
		switch(itArgv->dwType){
		case	GenericHTTPClient::TypeNormal:
			pBuffer=(PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE*4);

			_stprintf(	(TCHAR*)pBuffer,							
				TEXT("--%s\r\n")
				TEXT("Content-Disposition: form-data; name=\"%s\"\r\n")
				TEXT("\r\n")
				TEXT("%s\r\n"),
				szBoundary,
				itArgv->szName,
				itArgv->szValue);

			dwBufferSize=_tcslen((TCHAR*)pBuffer);

			break;
		case	GenericHTTPClient::TypeBinary:
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

			_stprintf( (TCHAR*)pBuffer,
				TEXT("--%s\r\n")
				TEXT("Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n")
				TEXT("Content-Type: %s\r\n")
				TEXT("\r\n"),
				szBoundary,
				itArgv->szName, itArgv->szValue,
				GetContentType(itArgv->szValue)
				);

			DWORD	dwBufPosition=_tcslen((TCHAR*)pBuffer);	

			while(::ReadFile(hFile, pBytes, __SIZE_BUFFER, &dwNumOfBytesToRead, NULL) && dwNumOfBytesToRead>0 && dwTotalBytes<=dwSize){
				::CopyMemory((pBuffer+dwBufPosition+dwTotalBytes), pBytes, dwNumOfBytesToRead);
				::ZeroMemory(pBytes, __SIZE_BUFFER+1);
				dwTotalBytes+=dwNumOfBytesToRead;				
			}

			::CopyMemory((pBuffer+dwBufPosition+dwTotalBytes), TEXT("\r\n"), _tcslen(TEXT("\r\n")));
			::CloseHandle(hFile);

			dwBufferSize=dwBufPosition+dwTotalBytes+_tcslen(TEXT("\r\n"));
			break;			
		}

		::CopyMemory((pInBuffer+dwPosition), pBuffer, dwBufferSize);
		dwPosition+=dwBufferSize;

		if(pBuffer){
			::LocalFree(pBuffer);
			pBuffer=NULL;
		}
	}

	::CopyMemory((pInBuffer+dwPosition), "--", 2);
	::CopyMemory((pInBuffer+dwPosition+2), szBoundary, _tcslen(szBoundary));
	::CopyMemory((pInBuffer+dwPosition+2+_tcslen(szBoundary)), "--\r\n", 3);

	return dwPosition+5+_tcslen(szBoundary);
}

DWORD GenericHTTPClient::GetMultiPartsFormDataLength(){
	std::vector<GenericHTTPArgument>::iterator itArgv;

	DWORD	dwLength=0;

	for(itArgv=_vArguments.begin(); itArgv<_vArguments.end(); ++itArgv){

		switch(itArgv->dwType){
		case	GenericHTTPClient::TypeNormal:
			dwLength+=__SIZE_HTTP_HEAD_LINE*4;
			break;
		case	GenericHTTPClient::TypeBinary:
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

LPCTSTR GenericHTTPClient::GetContentType(LPCTSTR szName)
{
	static TCHAR	szReturn[1024]= { 0 };
	LONG	dwLen=1024;
	DWORD	dwDot=0;

	for(dwDot=_tcslen(szName);dwDot>0;dwDot--){
		if(!_tcsncmp((szName+dwDot),TEXT("."), 1))
			break;
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
		_tcsncpy(szReturn, TEXT("application/octet-stream"), _tcslen(TEXT("application/octet-stream")));
	}

	return szReturn;
}

DWORD GenericHTTPClient::GetLastError()
{
	return m_dwError;
}

VOID GenericHTTPClient::ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, WORD &wPort, LPTSTR szURI){

	TCHAR szPort[__SIZE_BUFFER]= { 0 };
	DWORD dwPosition=0;
	BOOL bFlag=FALSE;

	while(_tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), TEXT(":"), 1))
		++dwPosition;

	if(!_tcsncmp((szURL+dwPosition+1), TEXT("/"), 1)){	// is PROTOCOL
		if(szProtocol){
			_tcsncpy(szProtocol, szURL, dwPosition);
			szProtocol[dwPosition]=0;
		}
		bFlag=TRUE;
	}else{	// is HOST 
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
	while(_tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), TEXT("/"), 1))
		++dwPosition;

	DWORD dwFind=dwStartPosition;

	for(;dwFind<=dwPosition;dwFind++){
		if(!_tcsncmp((szURL+dwFind), TEXT(":"), 1)){ // find PORT
			bFlag=TRUE;
			break;
		}
	}

	if(bFlag){
		TCHAR sztmp[__SIZE_BUFFER]= { 0 };
		_tcsncpy(sztmp, (szURL+dwFind+1), dwPosition-dwFind);
		wPort=_ttol(sztmp);
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwFind-dwStartPosition);
	}else if(!_tcscmp(szProtocol,TEXT("https"))){
		wPort=INTERNET_DEFAULT_HTTPS_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}else {
		wPort=INTERNET_DEFAULT_HTTP_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}

	if(dwPosition<_tcslen(szURL)){ // find URI
		_tcsncpy(szURI, (szURL+dwPosition), _tcslen(szURL)-dwPosition);
	}else{
		szURI[0]=0;
	}

	return;
}

LPCTSTR GenericHTTPClient::QueryHTTPResponseHeader(){
	return m_szHTTPResponseHeader;
}

LPCTSTR GenericHTTPClient::QueryHTTPResponse(){
	return m_szHTTPResponseHTML;
}
