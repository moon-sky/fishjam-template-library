#ifndef VISUAL_STUDIO_ADDIN_H
#define VISUAL_STUDIO_ADDIN_H
#pragma  once

/****************************************************************************************************
* Visual Studio 的插件
* VC6 -- 实现 IDSAddIn 接口的COM对象；通过自动化对象控制集成开发环境(如 Documents对象代表所有打开的文档,
*        Window对象代表一个打开的窗口),通过 OnConnection 方法传入代表整个VSIDE的 Application 对象.
*        #import "devshl.dll";IApplicationPtr pApp;pApp.CreateObject(_T("MSDEV.Application"));
****************************************************************************************************/

#endif //VISUAL_STUDIO_ADDIN_H