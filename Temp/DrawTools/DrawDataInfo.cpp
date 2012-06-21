#include "StdAfx.h"
#include "DrawDataInfo.h"
#include "ftlFunctional.h"

#include "DrawTools/DrawObject.h"

using namespace std;
#include <algorithm>


//////////////////////////////////////////////////////
/////////////// CTempFileManager   //////////////////////

CTempFileManager::CTempFileManager()
{

}

CTempFileManager::~CTempFileManager()
{
	std::vector<TEMPFILEINFO>::iterator it = m_arFile.begin();
	for (; it != m_arFile.end(); it++)
	{
		DeleteFile(it->strFileName);
	}
	m_arFile.clear();
}

CTempFileManager* CTempFileManager::Instance()
{
	static CTempFileManager TempFileManager;
	return &TempFileManager;
}

void CTempFileManager::AddTempFile(CString strFile)
{
	std::vector<TEMPFILEINFO>::iterator it = m_arFile.begin();
	for (; it != m_arFile.end(); it++)
	{
		if (it->strFileName.CompareNoCase(strFile) == 0)
		{
			it->nRefCount ++;
			break;
		}
	}
	if (it == m_arFile.end())
	{
		TEMPFILEINFO stTempFileInfo(strFile);
		m_arFile.push_back(stTempFileInfo);
	}
}

void CTempFileManager::RemoveTempFile(CString strFile)
{
	std::vector<TEMPFILEINFO>::iterator it = m_arFile.begin();
	for (; it != m_arFile.end(); it++)
	{
		if (it->strFileName.CompareNoCase(strFile) == 0)
		{
			it->nRefCount --;
			if (it->nRefCount == 0)
			{
				DeleteFile(it->strFileName);
				m_arFile.erase(it);
			}
			break;
		}
	}
}

//////////////////////////////////////////////////////
/////////////// CDrawDataInfo   //////////////////////


CDrawDataInfo::CDrawDataInfo(LPCTSTR strTempFileName)
{
	m_strTempFileName = strTempFileName;
	CTempFileManager::Instance()->AddTempFile(m_strTempFileName);
}

CDrawDataInfo::CDrawDataInfo(const DrawObjectList& listDrawObject, const DrawObjectList& listSelectObject, LPCTSTR strName, LPCTSTR strTempFileName)
{
	for (DrawObjectList::const_iterator itObject = listDrawObject.begin(); 
		itObject != listDrawObject.end(); ++itObject)
	{
		CDrawObject* pOldObject = *itObject;
		CDrawObject* pNewObject = pOldObject->Clone();
		m_listDrawObject.push_back(pNewObject);
		DrawObjectList::const_iterator it = find(listSelectObject.begin(), listSelectObject.end(), pOldObject);
		if (it != listSelectObject.end())
		{
			m_listSelectObject.push_back(pNewObject);
		}
	}
	m_strTempFileName = strTempFileName;
	CTempFileManager::Instance()->AddTempFile(m_strTempFileName);
	m_strName = strName;
}

CDrawDataInfo::~CDrawDataInfo(void)
{
	for_each(m_listDrawObject.begin(), m_listDrawObject.end(), FTL::ObjecteDeleter<CDrawObject*>());

	m_listDrawObject.clear();

	m_listSelectObject.clear();
	//DeleteFile(m_strTempFileName);
	CTempFileManager::Instance()->RemoveTempFile(m_strTempFileName);
}


LPCTSTR CDrawDataInfo::GetDataInfoName()
{
	return m_strName;
}

BOOL CDrawDataInfo::CopyDataInfo(DrawObjectList& listDrawObject,  DrawObjectList& listSelectObject, CString& strTempFileName)
{
	for_each(listDrawObject.begin(), listDrawObject.end(), FTL::ObjecteDeleter<CDrawObject*>());
	listDrawObject.clear();

	for (DrawObjectList::const_iterator itObject = m_listDrawObject.begin(); 
		itObject != m_listDrawObject.end(); ++itObject)
	{
		CDrawObject* pObject = *itObject;
		if (pObject)
		{
			CDrawObject* pNewObject = pObject->Clone();
			listDrawObject.push_back(pNewObject);

			DrawObjectList::const_iterator it = find(m_listSelectObject.begin(), m_listSelectObject.end(), pObject);
			if (it != m_listSelectObject.end())
			{
				listSelectObject.push_back(pNewObject);
			}
		}
	}
	listSelectObject.clear();
	strTempFileName = m_strTempFileName;
	return TRUE;

}