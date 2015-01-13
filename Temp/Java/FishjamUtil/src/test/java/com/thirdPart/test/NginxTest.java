package com.thirdPart.test;

/**********************************************************************************************************************************************
 * Nginx(http://nginx.org)
 *   高性能的，轻量级的HTTP Web 服务器 和 反向代理服务器及电子邮件 IMAP/POP3/SMTP 代理服务器
 *   等价于 Apache +
 * 
 * Windows下运行: 直接双击运行(会阻塞当前运行进程?) 或 start nginx, 然后 http://127.0.0.1/ 访问即可
 * 交互: nginx -s 命令。 如 stop(停止), reload(重新加载配置), quit(退出程序)
 * TODO: 帮助？
 * 
 * 配置(nginx.conf -- TODO: 不完整，且未测试):
 *   配置详解 -- http://www.open-open.com/doc/view/e5ea389039894143b0a0ef4f3faf846b
 *   1.静态服务 -- 如果请求的是以gif|jpg|jpeg|png|bmp|swf 结尾的文件，则从D:/software/upload 目录下加载
 *     location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)${ root D:/software/upload; expires 30d; }
 *   2.如请求的url以 jsp 结尾，自动重定向到 tomcat_server
 *     location ~ \.(jsp)?$ { 
 *        proxy_set_header Host  $host; 
 *        proxy_set_header X-Forwarded-For  $remote_addr;
 *        proxy_pass http://tomcat_server;
 *     }
**********************************************************************************************************************************************/

public class NginxTest {

}
