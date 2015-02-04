#ifndef COM_RGS_H
#define COM_RGS_H

/********************************************************************************************
* 使用 .rgs 文件(存储在资源文件里)进行COM组件的自注册 -- 使用 BNF(Backus-Nauer form)语法
*   通过 .rgs -> .rc 中的 REGISTRY -> DECLARE_REGISTRY_RESOURCEID 进行关联
* IRegistrar -- ATL提供的注册器组件(实现由 statreg.h 提供 )，使用宿主文件组件的信息来刷新注册表
*
* Idl和rgs的对应
*  HKCR
*    AppID -- COM Exe 使用? 有什么用?
*    
*   
* 关键字
*   NoRemove    -- 卸载COM时不删除，一般是系统的目录(比如 NoRemove CLSID)
*   ForceRemove -- 卸载COM时删除，一般是COM组件内部注册的部分，比如 ForceRemove {xxxx} = s 'MyMath Class'
* 
********************************************************************************************/

#endif //COM_RGS_H