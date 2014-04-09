#pragma once
#include "TabCtrlEx.h"

class CCastTesterTabCtrl : public CTabCtrlEx
{
public:
    CCastTesterTabCtrl(void);
    ~CCastTesterTabCtrl(void);
public:
    BOOL Init();
    BOOL UnInit();
};
