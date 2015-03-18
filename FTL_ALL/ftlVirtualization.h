#ifndef FTL_VIRTUALIZATION_H
#define FTL_VIRTUALIZATION_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVirtualization.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* 虚拟化技术
	*   虚拟机软件
	*     VirtualBox -- 开源免费(GPL协议)，体积小，支持Win/Linux,不需要像VMvare一样安装虚拟网卡。
	*     VMware Workstation -- 不同的OS之间还能互动操作(如 文件分享以及复制粘贴 等)
    *       和主机共享目录: 
	*     Virtual PC -- 
	*
	* VirtualBox开发环境的搭建详解 -- http://www.cnblogs.com/cxun/archive/2013/03/12/2956175.html
	*****************************************************************************************************/
    class CFVirtualization
    {
    public:
        FTLINLINE CFVirtualization(void);
        FTLINLINE virtual ~CFVirtualization(void);
    };

}

#endif //FTL_VIRTUALIZATION_H

#ifndef USE_EXPORT
#  include "ftlVirtualization.hpp"
#endif