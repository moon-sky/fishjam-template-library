#pragma once

#ifndef __AFXWIN_H__
#  error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "MainFrame.h"

//#include "resource.h"		// 主符号

class CTimeSaverApp : public CWinApp
{
public:
    CTimeSaverApp(void);
    ~CTimeSaverApp(void);

    // 重写
public:
    virtual BOOL InitInstance();
    // 实现
    DECLARE_MESSAGE_MAP()
private:
};

extern CTimeSaverApp theApp;
