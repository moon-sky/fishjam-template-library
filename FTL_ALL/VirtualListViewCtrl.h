#pragma once

#include <atlcoll.h>
#include <vector>

template <class E>
class CVirtualListViewCtrlT : public ATL::CWindowImpl<CVirtualListViewCtrlT<E>, CListViewCtrl>
{
public:
	BEGIN_MSG_MAP_EX(CVirtualListViewCtrlT)
		MSG_WM_NOTIFY(OnNotify)
	END_MSG_MAP()

	//HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
	//	DWORD dwStyle = 0, DWORD dwExStyle = 0,
	//	ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	//{
	//	dwStyle |=  LVS_REPORT | LVS_OWNERDATA;
	//	dwExStyle |= LVS_EX_FULLROWSELECT;
	//	return TBase::Create(GetWndClassName(), hWndParent, rect.m_lpRect, 
	//		szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);
	//}

	BOOL AddItem(const E& src)
	{
		//m_allValue.Append(src);
		//return SetItemCount(m_allValue.GetCount());
		m_Items.push_back(src);
		SetItemCount(m_Items.size());

		return TRUE;
	}

#ifndef HANDLE_CASE_RETURN_STRING
# define HANDLE_CASE_RETURN_STRING(c) \
case (c):\
	return TEXT(#c);
#endif 

	LPCTSTR GetNotifyCodeString(UINT code)
	{
		switch(code)
		{
			HANDLE_CASE_RETURN_STRING(NM_FIRST);
			HANDLE_CASE_RETURN_STRING(NM_OUTOFMEMORY);
			HANDLE_CASE_RETURN_STRING(NM_CLICK);
			HANDLE_CASE_RETURN_STRING(NM_DBLCLK);
			HANDLE_CASE_RETURN_STRING(NM_RETURN);
			HANDLE_CASE_RETURN_STRING(NM_RCLICK);
			HANDLE_CASE_RETURN_STRING(NM_RDBLCLK);
			HANDLE_CASE_RETURN_STRING(NM_SETFOCUS);
			HANDLE_CASE_RETURN_STRING(NM_KILLFOCUS);
			HANDLE_CASE_RETURN_STRING(NM_CUSTOMDRAW);
			HANDLE_CASE_RETURN_STRING(NM_HOVER);
			HANDLE_CASE_RETURN_STRING(NM_NCHITTEST);
			HANDLE_CASE_RETURN_STRING(NM_KEYDOWN);
			HANDLE_CASE_RETURN_STRING(NM_RELEASEDCAPTURE);
			HANDLE_CASE_RETURN_STRING(NM_SETCURSOR);
			HANDLE_CASE_RETURN_STRING(NM_CHAR);
			HANDLE_CASE_RETURN_STRING(NM_TOOLTIPSCREATED);
#if (_WIN32_IE >= 0x0500)
			HANDLE_CASE_RETURN_STRING(NM_LDOWN);
			HANDLE_CASE_RETURN_STRING(NM_RDOWN);
			HANDLE_CASE_RETURN_STRING(NM_THEMECHANGED);
#endif

#if (_WIN32_IE >= 0x0600)
			HANDLE_CASE_RETURN_STRING(NM_FONTCHANGED);
			HANDLE_CASE_RETURN_STRING(NM_CUSTOMTEXT);
			HANDLE_CASE_RETURN_STRING(NM_TVSTATEIMAGECHANGING);
#endif

			HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGING);
			HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGED);
			HANDLE_CASE_RETURN_STRING(LVN_INSERTITEM);
			HANDLE_CASE_RETURN_STRING(LVN_DELETEITEM);
			HANDLE_CASE_RETURN_STRING(LVN_DELETEALLITEMS);
			HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITA);
			HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITW);
			HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITA);
			HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITW);
			HANDLE_CASE_RETURN_STRING(LVN_COLUMNCLICK);
			HANDLE_CASE_RETURN_STRING(LVN_BEGINDRAG);
			HANDLE_CASE_RETURN_STRING(LVN_BEGINRDRAG);
#if (_WIN32_IE >= 0x0300)
			HANDLE_CASE_RETURN_STRING(LVN_ODCACHEHINT);
			HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMA);
			HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMW);
			HANDLE_CASE_RETURN_STRING(LVN_ITEMACTIVATE);
			HANDLE_CASE_RETURN_STRING(LVN_ODSTATECHANGED);
#endif      // _WIN32_IE >= 0x0300


#if (_WIN32_IE >= 0x0400)
			HANDLE_CASE_RETURN_STRING(LVN_HOTTRACK);
#endif
			HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOA);
			HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOW);
			HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOA);
			HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOW);

			//HANDLE_CASE_RETURN_STRING(XXXXXXXX);
			//HANDLE_CASE_RETURN_STRING(XXXXXXXX);
			//HANDLE_CASE_RETURN_STRING(XXXXXXXX);
			//HANDLE_CASE_RETURN_STRING(XXXXXXXX);
		}
		return TEXT("Unknown");
	}

	LRESULT OnNotify(int idCtrl, LPNMHDR pNMHDR)
	{
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		LV_ITEM* pItem = &(pDispInfo)->item;
		//if (idCtrl == IDC_LIST1)
		{
			ATLTRACE(TEXT("Code=%d(%s)\n"), pNMHDR->code, GetNotifyCodeString(pNMHDR->code));
		}
		if (pDispInfo->hdr.code == LVN_GETDISPINFO )
		{
			ATLTRACE(TEXT("LVN_GETDISPINFO, item=%d \n"), pItem->iItem);
			
			int iItem = pItem->iItem;

			if (pItem->mask & LVIF_TEXT) //valid text buffer?
			{
				switch(pItem->iSubItem)
				{
				case 0: //fill in main text
					_tcscpy_s(pItem->pszText, pItem->cchTextMax,
						m_Items[iItem].m_strItemText);
					break;
				case 1: //fill in sub item 1 text
					_tcscpy_s(pItem->pszText, pItem->cchTextMax,
						m_Items[iItem].m_strSubItem1Text);
					break;
				case 2: //fill in sub item 2 text
					_tcscpy_s(pItem->pszText, pItem->cchTextMax,
						m_Items[iItem].m_strSubItem2Text);
					break;
				}
			}
		}


		//if (pItem->mask & LVIF_IMAGE) //valid image?
		//{
		//	pItem->iImage = m_Items[iItem].m_iImage;
		//}
		return 0;
	}

protected:
	//CAtlArray<E>	m_allValue;
	std::vector<E>	m_Items;
};
