#pragma once

#ifdef DRAW_TOOL_TEST
#include "DrawTypeDefine.h"
#else
#include "DrawTools/DrawTypeDefine.h"
#endif 

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