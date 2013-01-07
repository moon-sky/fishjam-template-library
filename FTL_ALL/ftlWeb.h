#ifndef FTL_WEB_H
#define FTL_WEB_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlWeb.h requires ftlbase.h to be included first
#endif


/*************************************************************************************************************************
* DHTML -- Dynamic HTML
* 
*
* WebService
*   各应用程序通过网络协议和规定的一些标准数据格式（Http,XML,Soap)来访问WebService,通过其内部执行得到所需结果
*   基本的 Web Services 平台是 XML+HTTP，使用 XML 来编解码数据，并使用 SOAP 来传输数据
*   SOAP(Simple Object Access Protocol) -- 简单对象访问协议，是基于XML的协议，主要用于应用程序之间的通信
*   UDDI(Universal Description, Discovery and Integration) -- 通用描述、发现及整合，是一种存储WebService的目录服务(只在.NET中?)
*   WSDL(Web Services Description Language) -- 网络服务描述语言，基于XML，用于描述和定位WebServices
*   
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_WEB_H

#ifndef USE_EXPORT
#  include "ftlWeb.hpp"
#endif 