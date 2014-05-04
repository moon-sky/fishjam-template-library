#include "StdAfx.h"
#include "AtlEncTester.h"

#include <atlenc.h>
#include <atlutil.h>

CAtlEncTester::CAtlEncTester()
{
    //m_strOriginalURL = _T("http://microsoft.com/test/t%e<s t.asp?url=/{ex% ample</abc.asp?frame=true#fr%agment");
    m_strUrlEscaped  = _T("http://microsoft.com/test/t%e<s%20t.asp?url=/{ex% ample</abc.asp?frame=true#fr%agment");


    m_strOriginalURL = _T("{\"syncDate\":1398837962127,\"page\":1,\"commands\":{\"add\":[{\"content\":\"?+\+&+=\",\"important\"\
:false,\"registerDate\":\"2014-04-30T06:08:38Z\",\"colorId\":0,\"images\":[],\"modifyDate\":\"2014-04-30T06:08:38Z\"\
,\"clientId\":64,\"folderId\":805}]},\"changes\":20}");
    
}

void CAtlEncTester::test_AtlCanonicalizeUrl()
{
}

void CAtlEncTester::test_AtlCombineUrl()
{
    AtlCombineUrl
}

void CAtlEncTester::test_AtlEscapeUrl()
{
    BOOL bRet = FALSE;
    CFMemAllocator<TCHAR> bufEscaped;
    DWORD dwEscapedCount = bufEscaped.GetCount();
    DWORD dwFalgs = ATL_URL_ENCODE_PERCENT | ATL_URL_CANONICALIZE;

    bRet = AtlEscapeUrl(m_strOriginalURL, bufEscaped, &dwEscapedCount, dwEscapedCount, dwFalgs);
    if (!bRet && dwEscapedCount > bufEscaped.GetCount())
    {
        API_VERIFY(AtlEscapeUrl(m_strOriginalURL, bufEscaped.GetMemory(dwEscapedCount), &dwEscapedCount, dwEscapedCount, dwFalgs));
        FTLTRACE(TEXT("%s\n"), bufEscaped);
    }

}

void CAtlEncTester::test_AtlHexEncode()
{
}

void CAtlEncTester::test_AtlUnicodeToUTF8()
{
}

void CAtlEncTester::test_Base64Encode()
{
}

void CAtlEncTester::test_BEncode()
{
}

void CAtlEncTester::test_QEncode()
{
}

void CAtlEncTester::test_QPEncode()
{
}

void CAtlEncTester::test_UUDecode()
{
}

void CAtlEncTester::test_UrlEscape()
{
    HRESULT hr = E_FAIL;
    DWORD dwFlags = URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY;//URL_ESCAPE_SPACES_ONLY | URL_DONT_ESCAPE_EXTRA_INFO;
    CFMemAllocator<TCHAR> bufEscaped;
    DWORD dwEscapedCount = bufEscaped.GetCount();
    COM_VERIFY_EXCEPT1(UrlEscape(m_strOriginalURL, bufEscaped, &dwEscapedCount, dwFlags), 
        E_POINTER);
    CPPUNIT_ASSERT(E_POINTER == hr);

    if (E_POINTER == hr)
    {
        CPPUNIT_ASSERT(dwEscapedCount > bufEscaped.GetCount());
        COM_VERIFY(UrlEscape(m_strOriginalURL, bufEscaped.GetMemory(dwEscapedCount), &dwEscapedCount, dwFlags));
        //CPPUNIT_ASSERT(m_strUrlEscaped == bufEscaped);
        FTLTRACE(TEXT("%s\n"), bufEscaped);
    }
}


#if 0
	
class CURLEncode  
{
public:
	CStringA URLEncode(const CStringA& vData);
	CStringA URLDecode(const CStringA& vData);

public:
	CURLEncode();
	virtual ~CURLEncode();
private:
	static CStringA csUnsafeString;

	CStringA decToHex(char num, int radix);
	bool isUnsafe(char compareChar);
	CStringA convert(char val);
};

	
// HEX Values array
char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// UNSAFE String
CStringA CURLEncode::csUnsafeString= "\"<>%\\^[]`+$,@:;/!#?=&";

// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
CStringA CURLEncode::convert(char val) 
{
	CStringA csRet;
	csRet += "%";
	csRet += decToHex(val, 16);	
	return  csRet;
}

// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
CStringA CURLEncode::decToHex(char num, int radix)
{	
	int temp=0;	
	CStringA csTmp;
	int num_char;
	num_char = (int) num;
	
	// ISO-8859-1 
	// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
	// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
	if (num_char < 0)
		num_char = 256 + num_char;

	while (num_char >= radix)
    {
		temp = num_char % radix;
		num_char = (int)floor((double)num_char / radix);
		csTmp = hexVals[temp];
    }
	
	csTmp += hexVals[num_char];

	if(csTmp.GetLength() < 2)
	{
		csTmp += '0';
	}

	CStringA strdecToHex(csTmp);
	// Reverse the String
	strdecToHex.MakeReverse(); 
	
	return strdecToHex;
}

// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
// TRUE = UNSAFE, FALSE = SAFE
bool CURLEncode::isUnsafe(char compareChar)
{
	bool bcharfound = false;
	char tmpsafeChar;
	int m_strLen = 0;
	
	m_strLen = csUnsafeString.GetLength();
	for(int ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
	{
		tmpsafeChar = csUnsafeString.GetAt(ichar_pos); 
		if(tmpsafeChar == compareChar)
		{ 
			bcharfound = true;
			break;
		} 
	}
	int char_ascii_value = 0;
	//char_ascii_value = __toascii(compareChar);
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
	{
		return false;
	}
	// found no unsafe chars, return false		
	else
	{
		return true;
	}
	
	return true;
}
// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
// TO URL ENCODE FORM.
CStringA CURLEncode::URLEncode(const CStringA& pcsEncode)
{	
	int ichar_pos;
	CStringA csEncode;
	CStringA csEncoded;	
	int m_length;

	csEncode = pcsEncode;
	m_length = csEncode.GetLength();
	
	for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
	{
		char ch = csEncode.GetAt(ichar_pos);
		if (ch < ' ') 
		{
			ch = ch;
		}		
		if(!isUnsafe(ch))
		{
			// Safe Character				
			csEncoded += CStringA(ch);
		}
		else
		{
			// get Hex Value of the Character
			csEncoded += convert(ch);
		}
	}
	

	return csEncoded;

}
CStringA CURLEncode::URLDecode(const CStringA& pcsDecode)
{
	int ichar_pos;
	CStringA csDecode;
	CStringA csDecoded;	
	int m_length;

	csDecode = pcsDecode;
	m_length = csDecode.GetLength();
	
	for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
	{
		char ch = csDecode.GetAt(ichar_pos);
		if (ch < ' ') 
		{
			ch = ch;
		}		
		if(ch == '%')
		{
			char   *stopstring;
			char buf[2];
			buf[0] = csDecode.GetAt(ichar_pos+1);
			buf[1] = csDecode.GetAt(ichar_pos+2);	
			int ul = strtoul( buf, &stopstring, 16 );
			char ch = (char)ul;
			csDecoded += CStringA(ch);
			ichar_pos+=2;
		}
		else
		{
			csDecoded += CStringA(ch);
		}
	}
	

	return csDecoded;

}

#endif