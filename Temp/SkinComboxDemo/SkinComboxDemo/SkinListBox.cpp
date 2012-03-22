#include "StdAfx.h"
#include "SkinListBox.h"

CSkinListBox::CSkinListBox(void)
{
}

CSkinListBox::~CSkinListBox(void)
{
}

void CSkinListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    FTLTRACE(TEXT("CSkinListBox::DrawItem, iItem=%d\n"), lpDrawItemStruct->itemID);
}

void CSkinListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    FTLTRACE(TEXT("CSkinListBox::MeasureItem, iItem=%d\n"), lpMeasureItemStruct->itemID);
}

int  CSkinListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
    FTLTRACE(TEXT("CSkinListBox::CompareItem, iItem1=%d, iItem2=%d\n"), 
        lpCompareItemStruct->itemID1,
        lpCompareItemStruct->itemID2);

    return 0;
}

void CSkinListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
    FTLTRACE(TEXT("CSkinListBox::DeleteItem, iItem=%d\n"), lpDeleteItemStruct->itemID);
}
