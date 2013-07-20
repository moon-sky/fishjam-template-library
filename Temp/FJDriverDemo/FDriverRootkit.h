#ifndef F_DRIVER_ROOTKIT_H
#define F_DRIVER_ROOTKIT_H


/******************************************************************************************************************
* Rootkit -- 
*   Rootkit技术已经广泛应用于木马、安全软件、流氓软件中。现代木马几乎全部是Rootkit木马，而安全软件也几乎都应用了Rootkit技术
* ARK -- Anti-Rookit(反Rootkit工具)，典型代表是Icesword、DarkSpy、Rootkit Detector等
* HIPS(Host-based Intrusion Prevention System) -- 基于主机的入侵防御系统，是一种能监控你电脑中文件的运行和文件运用了
*   其他的文件以及文件对注册表的修改，并向你报告请求允许的的软件
*
*
* 获取Ring0权限
*   驱动、操作\Device\PhysicalMemory以设置调用门、修改win32k.sys 等
******************************************************************************************************************/


#endif //F_DRIVER_ROOTKIT_H