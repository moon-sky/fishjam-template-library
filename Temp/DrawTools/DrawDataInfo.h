#pragma once

#include "DrawTools\DrawTypeDefine.h"
#include <list>

#include <vector>

class CDrawDataInfo
{
public:
	CDrawDataInfo();
	CDrawDataInfo(const DrawObjectList& listDrawObject, const DrawObjectList& listSelectObject, LPCTSTR strName);
	~CDrawDataInfo(void);

	LPCTSTR GetDataInfoName();

	BOOL CopyDataInfo(DrawObjectList& listDrawObject,  DrawObjectList& listSelectObject);
private:
	DrawObjectList m_listDrawObject;
	DrawObjectList m_listSelectObject;

	CString        m_strName;
};


typedef std::vector<CDrawDataInfo*> CDrawDataArray;