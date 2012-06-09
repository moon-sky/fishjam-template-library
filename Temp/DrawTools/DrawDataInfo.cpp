#include "StdAfx.h"
#include "DrawDataInfo.h"
#include "ftlFunctional.h"

#include "DrawTools/DrawObject.h"
using namespace std;
#include <algorithm>


CDrawDataInfo::CDrawDataInfo()
{

}

CDrawDataInfo::CDrawDataInfo(const DrawObjectList& listDrawObject, const DrawObjectList& listSelectObject, LPCTSTR strName)
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
	m_strName = strName;
}

CDrawDataInfo::~CDrawDataInfo(void)
{
	for_each(m_listDrawObject.begin(), m_listDrawObject.end(), FTL::ObjecteDeleter<CDrawObject*>());

	m_listDrawObject.clear();

	m_listSelectObject.clear();
}


LPCTSTR CDrawDataInfo::GetDataInfoName()
{
	return m_strName;
}

BOOL CDrawDataInfo::CopyDataInfo(DrawObjectList& listDrawObject,  DrawObjectList& listSelectObject)
{
	listSelectObject.clear();

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
	return TRUE;

}