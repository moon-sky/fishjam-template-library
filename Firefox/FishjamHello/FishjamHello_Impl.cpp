#include <windows.h>

#include "FishjamHello_Impl.h"
#include "nsXPCOMStrings.h"

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHelloFishjam, nsIHelloFishjam)

nsHelloFishjam::nsHelloFishjam()
{
	/* member initializers and constructor code */
}

nsHelloFishjam::~nsHelloFishjam()
{
	/* destructor code */
}


NS_IMETHODIMP nsHelloFishjam::Hello(const nsAString & url)
{
	//MessageBox(NULL, TEXT("In nsHelloFishjam::Hello"), NULL, MB_OK);
	OutputDebugString(TEXT("In nsHelloFishjam::Hello\n"));
	return NS_OK;
}

/* long Add (in long num1, in long num2); */
NS_IMETHODIMP nsHelloFishjam::Add(PRInt32 num1, PRInt32 num2, PRInt32 *_retval NS_OUTPARAM)
{
	if (!_retval)
	{
		return NS_ERROR_NULL_POINTER;
	}
	*_retval = (num1 + num2);
	return NS_OK;
}

/* void AddEx (in long num1, in long num2, out long result); */
NS_IMETHODIMP nsHelloFishjam::AddEx(PRInt32 num1, PRInt32 num2, PRInt32 *result NS_OUTPARAM)
{
	if (!result)
	{
		return NS_ERROR_NULL_POINTER;
	}
	*result = num1 + num2;
	return NS_OK;
}

/* readonly attribute AString Version; */
NS_IMETHODIMP nsHelloFishjam::GetVersion(nsAString & aVersion)
{
	aVersion = TEXT("1.0.0.0");
	return NS_OK;
}