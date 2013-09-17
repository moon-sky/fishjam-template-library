#pragma once
#include "TabCtrlEx.h"

class COpenGLDemoTabCtrls : public CTabCtrlEx
{
public:
    COpenGLDemoTabCtrls(void);
    ~COpenGLDemoTabCtrls(void);
public:
    BOOL Init();
    BOOL UnInit();
};
