#include <comdef.h> 
#pragma once 

// 
// http://www.sellsbrothers.com/tools/CComSafeArray.zip 
// 
// 
///////////////////////////////////////////////////////////////////////////// 
// CSafeArray class 

typedef const SAFEARRAY* LPCSAFEARRAY; 
typedef const VARIANT* LPCVARIANT; 

class CComSafeArray : public tagVARIANT 
{ 
//Constructors 
public: 
	CComSafeArray(); 
	CComSafeArray(const SAFEARRAY& saSrc, VARTYPE vtSrc); 
	CComSafeArray(LPCSAFEARRAY pSrc, VARTYPE vtSrc); 
	CComSafeArray(const CComSafeArray& saSrc); 
	CComSafeArray(const VARIANT& varSrc); 
	CComSafeArray(LPCVARIANT pSrc); 
//	CComSafeArray(const _variant_t& varSrc); 

// Operations 
public: 
	void Clear(); 
	void Attach(VARIANT& varSrc); 
	VARIANT Detach(); 

	CComSafeArray& operator=(const CComSafeArray& saSrc); 
	CComSafeArray& operator=(const VARIANT& varSrc); 
	CComSafeArray& operator=(LPCVARIANT pSrc); 
	CComSafeArray& operator=(const _variant_t& varSrc); 

	BOOL operator==(const SAFEARRAY& saSrc) const; 
	BOOL operator==(LPCSAFEARRAY pSrc) const; 
	BOOL operator==(const CComSafeArray& saSrc) const; 
	BOOL operator==(const VARIANT& varSrc) const; 
	BOOL operator==(LPCVARIANT pSrc) const; 
	BOOL operator==(const _variant_t& varSrc) const; 

	operator LPVARIANT(); 
	operator LPCVARIANT() const; 

	// One dim array helpers 
	void CreateOneDim(VARTYPE vtSrc, DWORD dwElements, 
		const void* pvSrcData = NULL, long nLBound = 0); 
	DWORD GetOneDimSize(); 
	void ResizeOneDim(DWORD dwElements); 

	// Multi dim array helpers 
	void Create(VARTYPE vtSrc, DWORD dwDims, DWORD* rgElements); 

	// SafeArray wrapper classes 
	void Create(VARTYPE vtSrc, DWORD dwDims, SAFEARRAYBOUND* rgsabounds); 
	void AccessData(void** ppvData); 
	void UnaccessData(); 
	void AllocData(); 
	void AllocDescriptor(DWORD dwDims); 
	void Copy(LPSAFEARRAY* ppsa); 
	void GetLBound(DWORD dwDim, long* pLBound); 
	void GetUBound(DWORD dwDim, long* pUBound); 
	void GetElement(long* rgIndices, void* pvData); 
	void PtrOfIndex(long* rgIndices, void** ppvData); 
	void PutElement(long lIndex, void* pvData); 
	void PutElement(long* rgIndices, void* pvData); 
	void Redim(SAFEARRAYBOUND* psaboundNew); 
	void Lock(); 
	void Unlock(); 
	DWORD GetDim(); 
	DWORD GetElemSize(); 
	void Destroy(); 
	void DestroyData(); 
	void DestroyDescriptor(); 

//Implementation 
public: 
	DWORD GetSize(DWORD nDim); 
	~CComSafeArray(); 

	// Cache info to make element access (operator []) faster 
	DWORD m_dwElementSize; 
	DWORD m_dwDims; 

private: 
}; 

///////////////////////////////////////////////////////////////////////////// 
// Helper for initializing CComSafeArray 
inline void ATLSafeArrayInit(CComSafeArray* psa); 

inline BOOL ATLCompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2); 

inline void ATLCopyBinaryData(SAFEARRAY* parray, const void* pvSrc, DWORD dwSize); 

inline void ATLCreateOneDimArray(VARIANT& varSrc, DWORD dwSize); 

///////////////////////////////////////////////////////////////////////////// 
// CComSafeArray class 

inline CComSafeArray::CComSafeArray() 
{ 
	ATLSafeArrayInit(this);
	vt = VT_EMPTY; 
}

inline CComSafeArray::CComSafeArray(const SAFEARRAY& saSrc, VARTYPE vtSrc)
{
	ATLSafeArrayInit(this);
	vt = (VARTYPE)(vtSrc | VT_ARRAY);
	_com_util::CheckError(::SafeArrayCopy((LPSAFEARRAY)&saSrc, &parray));
	m_dwDims = GetDim();
	m_dwElementSize = GetElemSize();
}

inline CComSafeArray::CComSafeArray(LPCSAFEARRAY pSrc, VARTYPE vtSrc)
{
	ATLSafeArrayInit(this);
	vt = (VARTYPE)(vtSrc | VT_ARRAY);
	_com_util::CheckError(::SafeArrayCopy((LPSAFEARRAY)pSrc, &parray));
	m_dwDims = GetDim();
	m_dwElementSize = GetElemSize();
}

inline CComSafeArray::CComSafeArray(const CComSafeArray& saSrc)
{
	ATLSafeArrayInit(this);
	*this = saSrc;
	m_dwDims = GetDim();
	m_dwElementSize = GetElemSize();
}

inline CComSafeArray::CComSafeArray(const VARIANT& varSrc)
{
	ATLSafeArrayInit(this);
	*this = varSrc;
	m_dwDims = GetDim();
	m_dwElementSize = GetElemSize();
}

inline CComSafeArray::CComSafeArray(LPCVARIANT pSrc)
{
	ATLSafeArrayInit(this);
	*this = pSrc;
	m_dwDims = GetDim();
	m_dwElementSize = GetElemSize();
}

// Operations
inline void CComSafeArray::Attach(VARIANT& varSrc)
{
	_ASSERT(varSrc.vt & VT_ARRAY);

	// Free up previous safe array if necessary
	Clear();

	// give control of data to CComSafeArray
	memcpy(this, &varSrc, sizeof(varSrc));
	varSrc.vt = VT_EMPTY;
}

inline VARIANT CComSafeArray::Detach()
{
	VARIANT varResult = *this;
	vt = VT_EMPTY;
	return varResult;
}

// Assignment operators
inline CComSafeArray& CComSafeArray::operator=(const CComSafeArray& saSrc)
{
	_ASSERT(saSrc.vt & VT_ARRAY);

	_com_util::CheckError(::VariantCopy(this, (LPVARIANT)&saSrc));
	return *this;
}

inline CComSafeArray& CComSafeArray::operator=(const VARIANT& varSrc)
{
	_ASSERT(varSrc.vt & VT_ARRAY);

	_com_util::CheckError(::VariantCopy(this, (LPVARIANT)&varSrc));
	return *this;
}

inline CComSafeArray& CComSafeArray::operator=(LPCVARIANT pSrc)
{
	_ASSERT(pSrc->vt & VT_ARRAY);

	_com_util::CheckError(::VariantCopy(this, (LPVARIANT)pSrc));
	return *this;
}

inline CComSafeArray& CComSafeArray::operator=(const _variant_t& varSrc)
{
	_ASSERT(varSrc.vt & VT_ARRAY);

	_com_util::CheckError(::VariantCopy(this, (LPVARIANT)&varSrc));
	return *this;
}

// Comparison operators
inline BOOL CComSafeArray::operator==(const SAFEARRAY& saSrc) const
{
	return ATLCompareSafeArrays(parray, (LPSAFEARRAY)&saSrc);
}

inline BOOL CComSafeArray::operator==(LPCSAFEARRAY pSrc) const
{
	return ATLCompareSafeArrays(parray, (LPSAFEARRAY)pSrc);
}

inline BOOL CComSafeArray::operator==(const CComSafeArray& saSrc) const
{
	if (vt != saSrc.vt)
		return FALSE;

	return ATLCompareSafeArrays(parray, saSrc.parray);
}

inline BOOL CComSafeArray::operator==(const VARIANT& varSrc) const
{
	if (vt != varSrc.vt)
		return FALSE;

	return ATLCompareSafeArrays(parray, varSrc.parray);
}

inline BOOL CComSafeArray::operator==(LPCVARIANT pSrc) const
{
	if (vt != pSrc->vt)
		return FALSE;

	return ATLCompareSafeArrays(parray, pSrc->parray);
}

inline BOOL CComSafeArray::operator==(const _variant_t& varSrc) const
{
	if (vt != varSrc.vt)
		return FALSE;

	return ATLCompareSafeArrays(parray, varSrc.parray);
}

inline void CComSafeArray::CreateOneDim(VARTYPE vtSrc, DWORD dwElements,
	const void* pvSrcData, long nLBound)
{
	_ASSERT(dwElements > 0);

	// Setup the bounds and create the array
	SAFEARRAYBOUND rgsabound;
	rgsabound.cElements = dwElements;
	rgsabound.lLbound = nLBound;
	Create(vtSrc, 1, &rgsabound);

	// Copy over the data if necessary
	if (pvSrcData != NULL)
	{
		void* pvDestData;
		AccessData(&pvDestData);
		memcpy(pvDestData, pvSrcData, GetElemSize() * dwElements);
		UnaccessData();
	}
}

inline DWORD CComSafeArray::GetOneDimSize()
{
	_ASSERT(GetDim() == 1);

	return GetSize(1);
}

inline DWORD CComSafeArray::GetSize(DWORD nDim)
{
	_ASSERT(nDim > 0 && nDim <= GetDim());

	long nUBound, nLBound;

	GetUBound(nDim, &nUBound);
	GetLBound(nDim, &nLBound);

	return nUBound + 1 - nLBound;
}

inline void CComSafeArray::ResizeOneDim(DWORD dwElements)
{
	_ASSERT(GetDim() == 1);

	SAFEARRAYBOUND rgsabound;

	rgsabound.cElements = dwElements;
	rgsabound.lLbound = 0;

	Redim(&rgsabound);
}

inline void CComSafeArray::Create(VARTYPE vtSrc, DWORD dwDims, DWORD* rgElements)
{
	_ASSERT(rgElements != NULL);

	// Allocate and fill proxy array of bounds (with lower bound of zero)
	SAFEARRAYBOUND* rgsaBounds = new SAFEARRAYBOUND[dwDims];

	for (DWORD dwIndex = 0; dwIndex < dwDims; dwIndex++)
	{
		// Assume lower bound is 0 and fill in element count
		rgsaBounds[dwIndex].lLbound = 0;
		rgsaBounds[dwIndex].cElements = rgElements[dwIndex];
	}

	try
	{
		Create(vtSrc, dwDims, rgsaBounds);
	}
	catch(_com_error & e)
	{
		// Must free up memory
		delete[] rgsaBounds;
		_com_raise_error(e.Error());
	}

	delete[] rgsaBounds;
}

inline void CComSafeArray::Create(VARTYPE vtSrc, DWORD dwDims, SAFEARRAYBOUND* rgsabound)
{
	_ASSERT(dwDims > 0);
	_ASSERT(rgsabound != NULL);

	// Validate the VARTYPE for SafeArrayCreate call
	_ASSERT(!(vtSrc & VT_ARRAY));
	_ASSERT(!(vtSrc & VT_BYREF));
	_ASSERT(!(vtSrc & VT_VECTOR));
	_ASSERT(vtSrc != VT_EMPTY);
	_ASSERT(vtSrc != VT_NULL);

	// Free up old safe array if necessary
	Clear();

	parray = ::SafeArrayCreate(vtSrc, dwDims, rgsabound);

	if (parray == NULL)
		_com_raise_error(E_OUTOFMEMORY);

	vt = unsigned short(vtSrc | VT_ARRAY);
	m_dwDims = dwDims;
	m_dwElementSize = GetElemSize();
}

inline void CComSafeArray::AccessData(void** ppvData)
{
	_com_util::CheckError(::SafeArrayAccessData(parray, ppvData));
}

inline void CComSafeArray::UnaccessData()
{
	_com_util::CheckError(::SafeArrayUnaccessData(parray));
}

inline void CComSafeArray::AllocData()
{
	_com_util::CheckError(::SafeArrayAllocData(parray));
}

inline void CComSafeArray::AllocDescriptor(DWORD dwDims)
{
	_com_util::CheckError(::SafeArrayAllocDescriptor(dwDims, &parray));
}

inline void CComSafeArray::Copy(LPSAFEARRAY* ppsa)
{
	_com_util::CheckError(::SafeArrayCopy(parray, ppsa));
}

inline void CComSafeArray::GetLBound(DWORD dwDim, long* pLbound)
{
	_com_util::CheckError(::SafeArrayGetLBound(parray, dwDim, pLbound));
}

inline void CComSafeArray::GetUBound(DWORD dwDim, long* pUbound)
{
	_com_util::CheckError(::SafeArrayGetUBound(parray, dwDim, pUbound));
}

inline void CComSafeArray::GetElement(long* rgIndices, void* pvData)
{
	_com_util::CheckError(::SafeArrayGetElement(parray, rgIndices, pvData));
}

inline void CComSafeArray::PtrOfIndex(long* rgIndices, void** ppvData)
{
	_com_util::CheckError(::SafeArrayPtrOfIndex(parray, rgIndices, ppvData));
}

inline void CComSafeArray::PutElement(long lIndex, void* pvData)
{
	_ASSERT(GetDim() == 1);

	_com_util::CheckError(::SafeArrayPutElement(parray, &lIndex, pvData));
}

inline void CComSafeArray::PutElement(long* rgIndices, void* pvData)
{
	_com_util::CheckError(::SafeArrayPutElement(parray, rgIndices, pvData));
}

inline void CComSafeArray::Redim(SAFEARRAYBOUND* psaboundNew)
{
	_com_util::CheckError(::SafeArrayRedim(parray, psaboundNew));
}

inline void CComSafeArray::Lock()
{
	_com_util::CheckError(::SafeArrayLock(parray));
}

inline void CComSafeArray::Unlock()
{
	_com_util::CheckError(::SafeArrayUnlock(parray));
}

inline void CComSafeArray::Destroy()
{
	_com_util::CheckError(::SafeArrayDestroy(parray));
}

inline void CComSafeArray::DestroyData()
{
	_com_util::CheckError(::SafeArrayDestroyData(parray));
}

inline void CComSafeArray::DestroyDescriptor()
{
	_com_util::CheckError(::SafeArrayDestroyDescriptor(parray));
}

inline CComSafeArray::~CComSafeArray()
{ 
	Clear(); 
}

inline void CComSafeArray::Clear()
{ 
	// if it is an array
	if (vt & VT_ARRAY)
	{
		if (parray)
			Destroy();
		ATLSafeArrayInit(this); 
	}
	else
		_com_util::CheckError(::VariantClear(this)); 
}

inline CComSafeArray::operator LPVARIANT()
{ 
	return this; 
}

inline CComSafeArray::operator LPCVARIANT() const
{ 
	return this; 
}

inline DWORD CComSafeArray::GetDim()
{
	return ::SafeArrayGetDim(parray); 
}

inline DWORD CComSafeArray::GetElemSize()
{ 
	return ::SafeArrayGetElemsize(parray); 
}

/////////////////////////////////////////////////////////////////////////////// 
// CComSafeArray Helpers 
inline void ATLSafeArrayInit(CComSafeArray* psa)
{
	memset(psa, 0, sizeof(*psa));
}

inline BOOL ATLCompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2)
{
	BOOL bCompare = FALSE;

	// If one is NULL they must both be NULL to compare
	if (parray1 == NULL || parray2 == NULL)
	{
		return parray1 == parray2;
	}

	// Dimension must match and if 0, then arrays compare
	DWORD dwDim1 = ::SafeArrayGetDim(parray1);
	DWORD dwDim2 = ::SafeArrayGetDim(parray2);
	if (dwDim1 != dwDim2)
		return FALSE;
	else if (dwDim1 == 0)
		return TRUE;

	// Element size must match
	DWORD dwSize1 = ::SafeArrayGetElemsize(parray1);
	DWORD dwSize2 = ::SafeArrayGetElemsize(parray2);
	if (dwSize1 != dwSize2)
		return FALSE;

	long* pLBound1 = NULL;
	long* pLBound2 = NULL;
	long* pUBound1 = NULL;
	long* pUBound2 = NULL;

	void* pData1 = NULL;
	void* pData2 = NULL;

	try
	{
		// Bounds must match
		pLBound1 = new long[dwDim1];
		pLBound2 = new long[dwDim2];
		pUBound1 = new long[dwDim1];
		pUBound2 = new long[dwDim2];

		size_t nTotalElements = 1;

		// Get and compare bounds
		for (DWORD dwIndex = 0; dwIndex < dwDim1; dwIndex++)
		{
			_com_util::CheckError(::SafeArrayGetLBound(
				parray1, dwIndex+1, &pLBound1[dwIndex]));
			_com_util::CheckError(::SafeArrayGetLBound(
				parray2, dwIndex+1, &pLBound2[dwIndex]));
			_com_util::CheckError(::SafeArrayGetUBound(
				parray1, dwIndex+1, &pUBound1[dwIndex]));
			_com_util::CheckError(::SafeArrayGetUBound(
				parray2, dwIndex+1, &pUBound2[dwIndex]));

			// Check the magnitude of each bound
			if (pUBound1[dwIndex] - pLBound1[dwIndex] !=
				pUBound2[dwIndex] - pLBound2[dwIndex])
			{
				delete[] pLBound1;
				delete[] pLBound2;
				delete[] pUBound1;
				delete[] pUBound2;

				return FALSE;
			}

			// Increment the element count
			nTotalElements *= pUBound1[dwIndex] - pLBound1[dwIndex] + 1;
		}

		// Access the data
		_com_util::CheckError(::SafeArrayAccessData(parray1, &pData1));
		_com_util::CheckError(::SafeArrayAccessData(parray2, &pData2));

		// Calculate the number of bytes of data and compare
		size_t nSize = nTotalElements * dwSize1;
		int nOffset = memcmp(pData1, pData2, nSize);
		bCompare = nOffset == 0;

		// Release the array locks
		_com_util::CheckError(::SafeArrayUnaccessData(parray1));
		_com_util::CheckError(::SafeArrayUnaccessData(parray2));
	}
	catch(_com_error & e)
	{
		// Clean up bounds arrays
		delete[] pLBound1;
		delete[] pLBound2;
		delete[] pUBound1;
		delete[] pUBound2;

		// Release the array locks
		if (pData1 != NULL)
			_com_util::CheckError(::SafeArrayUnaccessData(parray1));
		if (pData2 != NULL)
			_com_util::CheckError(::SafeArrayUnaccessData(parray2));

		_com_raise_error(e.Error());
	}

	// Clean up bounds arrays
	delete[] pLBound1;
	delete[] pLBound2;
	delete[] pUBound1;
	delete[] pUBound2;

	return bCompare;
}

inline void ATLCreateOneDimArray(VARIANT& varSrc, DWORD dwSize)
{
	UINT nDim;

	// Clear VARIANT and re-create SafeArray if necessary
	if (varSrc.vt != (VT_UI1 | VT_ARRAY) ||
		(nDim = ::SafeArrayGetDim(varSrc.parray)) != 1)
	{
		_com_util::CheckError(::VariantClear(&varSrc));
		varSrc.vt = VT_UI1 | VT_ARRAY;

		SAFEARRAYBOUND bound;
		bound.cElements = dwSize;
		bound.lLbound = 0;
		varSrc.parray = ::SafeArrayCreate(VT_UI1, 1, &bound);
		if (varSrc.parray == NULL)
			_com_raise_error(E_OUTOFMEMORY);
	}
	else
	{
		// Must redimension array if necessary
		long lLower = 0, lUpper = 0;
		_com_util::CheckError(::SafeArrayGetLBound(varSrc.parray, 1, &lLower));
		_com_util::CheckError(::SafeArrayGetUBound(varSrc.parray, 1, &lUpper));

		// Upper bound should always be greater than lower bound
		long lSize = lUpper - lLower;
		if (lSize < 0)
		{
			_ASSERT(FALSE);
			lSize = 0;
		}

		if ((DWORD)lSize != dwSize)
		{
			SAFEARRAYBOUND bound;
			bound.cElements = dwSize;
			bound.lLbound = lLower;
			_com_util::CheckError(::SafeArrayRedim(varSrc.parray, &bound));
		}
	}
}

inline void ATLCopyBinaryData(SAFEARRAY* parray, const void* pvSrc, DWORD dwSize)
{
	// Access the data, copy it and unaccess it.
	void* pDest = NULL;
	_com_util::CheckError(::SafeArrayAccessData(parray, &pDest));
	memcpy(pDest, pvSrc, dwSize);
	_com_util::CheckError(::SafeArrayUnaccessData(parray));
}

