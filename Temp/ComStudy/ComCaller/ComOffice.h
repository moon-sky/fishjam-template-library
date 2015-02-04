#ifndef COM_OFFICE_H
#define COM_OFFICE_H

/***************************************************************************************************
* 问题：
*   1.注意用户安装多个版本的Office时 -- 各个版本的函数参数个数、类型不同
*     如 Office 2000 时是 Office\MSO9.dll
*        Office XP   时是 Office11\MSO.DLL
*        Office 2007 时是 Office12\MSO.DLL
*
* Microsoft 的 Office 产品中，都提供了OLE Automation 自动化程序的接口。
* 技巧 -- 由于函数的参数通常都是VARIANT类型的指针，可以通过 Office 中自带的“宏”功能确定具体的参数类型,
*   再使用 CComVariant 定义对应的变量。
*   如录制的宏：
*     ActiveDocument.SaveAs FileName:="Hello.doc", FileFormat:=wdFormatDocument, _
*       LockComments:=False, Password:="", AddToRecentFiles:=True, _
*       WritePassword:="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts:=False, _
*       SaveNativePictureFormat:=False, SaveFormsData:=False, SaveAsAOCELetter:= False
*   
* 
* Microsoft 把应用(Application)按逻辑功能划分为如下的树形结构
*   Application(WORD 为例，只列出一部分）
*   +- Documents（所有的文档）
*      +- Document（一个文档）
*   +- Templates（所有模板）
*      +- Template（一个模板）
*   +- Windows（所有窗口）
*      +- Window
*      +- Selection
*      +- View
*   +- Selection（编辑对象）
*      +- Font
*      +- Style
*      +- Range
*
* 使用WORD
*   1.#import MSOX.dll 接口库(注意可能需要 rename, raw_interfaces_only 等)
*   2.CComPtr<Word::_Application> m_pApp; 
*     COM_VERIFY(m_pApp.CoCreateInstance(__uuidof(Word::Application), NULL, CLSCTX_SERVER));
***************************************************************************************************/

#endif //COM_OFFICE_H