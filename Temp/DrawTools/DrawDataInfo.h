#pragma once

#include "DrawTools/DrawTypeDefine.h"

#include <list>

#include <vector>

typedef struct stu_TempFileInfo
{
	int     nRefCount;
	CString strFileName;

	stu_TempFileInfo(CString strFile)
	{
		nRefCount     = 1;
		strFileName   = strFile;
	}
}TEMPFILEINFO, *LPTEMPFILEINFO;

class CTempFileManager
{
public:
	static CTempFileManager* Instance();
	void AddTempFile(CString strFile);
	void RemoveTempFile(CString strFile);
private:
	CTempFileManager();
	~CTempFileManager();

	std::vector<TEMPFILEINFO> m_arFile;
};

class CDrawDataInfo
{
public:
	CDrawDataInfo(LPCTSTR strTempFileName);
	CDrawDataInfo(const DrawObjectList& listDrawObject, const DrawObjectList& listSelectObject, LPCTSTR strName, LPCTSTR strTempFileName);
	~CDrawDataInfo(void);

	LPCTSTR GetDataInfoName();

	BOOL CopyDataInfo(DrawObjectList& listDrawObject,  DrawObjectList& listSelectObject, CString& strTempFileName);
	BOOL GetDateInfo(DrawObjectList& listDrawObject,  DrawObjectList& listSelectObject, CString& strTempFileName);
	void GetClipRect(CRect& rcClip);
private:
	DrawObjectList m_listDrawObject;
	DrawObjectList m_listSelectObject;

	CString        m_strName;
	CString        m_strTempFileName;
};


typedef std::vector<CDrawDataInfo*> CDrawDataArray;