#pragma once
#include "TabCtrlEx.h"

class CWTLControlsTabCtrl : public CTabCtrlEx
{
public:
    CWTLControlsTabCtrl(void);
    ~CWTLControlsTabCtrl(void);
public:
    BOOL Init();
    BOOL UnInit();
};
