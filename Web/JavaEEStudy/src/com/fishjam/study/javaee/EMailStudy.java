package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * 邮件客户端(收发邮件)

 * JMai(javax.mail.*) -- 官方提供，但未包含在SDK中，需要单独下载，需要 JAF 的支持
 *    使用: a.加入 CLASSPATH 或 拷贝到 JAVA_HOME/lib/ext 下;
 *            b.核心类: Session,Message,Address,Authenticator,Transport,Store,Folder
 *            
 * Spring Mail(org.springframework.mail.*) -- Spring提供的收发电子邮件的高级抽象层
 *    软件包: activation.jar + mail.jar + spring.jar
 *    常见类和接口:
 *       MailSender -- 发送电子邮件的主要接口
 *       SimpleMailMessage -- 封装了简单邮件属性的值对象 
 *    
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 邮件服务器
 *    James(Java Apache Mail Enterprise Server) -- 是一种独立的邮件服务器，并提供了一个完整的电子邮件解决方案，用来收、发电子邮件，
 *       实现了SMTP、POP3与NNTP等多种邮件相关协议，可以通过Mailet扩充其功能(Mail2SMS、Mail2Fax等)。
 *       可以选择在文件、数据库或其他介质中保存邮件内容等。
 *       安装:
 *          0.安装配置 Java;
 *          0.5. 下载 JMail API, JAF API ?
 *          1.http://james.apache.org/ 下载zip包，解压，运行 bin\run.bat 即可， 
 *       配置(SAR-INF\config.xml, 启动一次后自动生成?) -- 默认 SMTP为25，POP3为110，NNTP为119，能在本机上收发邮件，但不能收发到外网邮件
 *          <postmaster> 《== 
 *          <servernames>
 *            autodetectIP|autodetect = "false" 《== 把自动探测IP属性设置为"false"， 为什么?
 *            <servername> 《== 邮箱域名，如 fishjam.com
 *            <authRequired> -- 控制是否需要账号验证
 *       创建邮件账号(James的账号管理是通过基于Telnet客户机的远程管理器 <== telnet localhost 4555 , 用户名/密码都为 root)
 *          help 查看帮助; adduser 添加账号; setforwarding 转发用户邮件; user 改变另一个用户库(能否改到DB中？)  
 *          
 *       mailet
 *          ToProcessor -- ?
**************************************************************************************************************************************/
public class EMailStudy {

}
