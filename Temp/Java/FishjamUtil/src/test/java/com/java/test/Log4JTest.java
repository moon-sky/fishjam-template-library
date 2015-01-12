package com.java.test;

import static org.junit.Assert.assertEquals;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.log4j.Appender;
import org.apache.log4j.ConsoleAppender;
import org.apache.log4j.FileAppender;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.log4j.PatternLayout;
import org.junit.Test;

/*****************************************************************************************************************************
* http://blog.csdn.net/seraph5186/article/details/3863726
* 
* Log4J
*   log4j提供分级方法在程序中嵌入日志记录语句。
*   日志信息具有多种输出格式和多个。
* 
* 配置
*   1.建立 log4j.properties 文件，在其中写入详细的配置键值对，如：
*     log4j.rootLogger=warn, A1
*     log4j.logger.org.springframework=warn  -- 
*     log4j.appender.A1=org.apache.log4j.ConsoleAppender
*   
* 输出级别(只输出等于或高于特定级别的信息) -- [all] < debug < info < warn < error < fatal  <[off]
*    
* 基本概念
*    Logger -- 负责处理日志记录的大部分操作
*    
*    Appender -- 负责控制日志记录操作的输出
*      ConsoleAppender -- 输出到[System.out]或者System.err
*      DailyRollingFileAppender -- 多个日志文件可以以一个用户选定的频率进行循环日志记录。
*      FileAppender -- 把日志事件写入文件
*      RollingFileAppender -- 扩展FileAppender备份容量达到一定大小(MaxFileSize)的日志文件, 最大个数通过 MaxBackupIndex 设置
*      WriterAppender -- 根据用户的选择把日志事件写入到Writer或者OutputStream
*      SMTPAppender -- 当特定的日志事件发生时，一般是指发生错误或者重大错误时，发送一封邮件
*      SocketAppender -- 给远程日志服务器（通常是网络套接字节点）发送日志事件（LoggingEvent）对象
*      SocketHubAppender -- 给远程日志服务器群组(通常是网络套接字节点)发送日志事件（LoggingEvent）对象
*      SyslogAppender -- 给远程异步日志记录的后台精灵程序(daemon)发送消息
*      TelnetAppender  -- 向只读网络套接字发送消息
*      <实现Appender接口，创建自定义方式的Appender>
*      
*    Layout -- 负责格式化Appender的输出
*      HTMLLayout -- 格式化日志输出为HTML表格
*      PatternLayout -- 根据指定的 转换模式格式化日志输出，或者如果没有指定任何转换模式，就使用默认的转换模式
*        如:ConversionPattern=[%p]%-d{yyyy-MM-dd HH:mm:ss SSS} [%c] %m%n
*      SimpleLayout -- 以一种非常简单的方式格式化日志输出，它打印级别 Level，然后跟着一个破折号“-“ ，最后才是日志消息
 ******************************************************************************************************************************/
public class Log4JTest {

	@Test
	public void testLogger(){
		assertEquals(1, 1);

		try {
			Appender consoleAppender = new ConsoleAppender(new PatternLayout());
			Appender fileAppender = new FileAppender(new PatternLayout(), "Log4JTest.log", false);
			
			Logger rootLogger = Logger.getRootLogger();
			rootLogger.addAppender(consoleAppender);
			dumpInfoThroughLogger(rootLogger, "getRootLogger");
			
			Logger nameLogger = Logger.getLogger("nameLogger");
			nameLogger.setLevel(Level.INFO);
			nameLogger.addAppender(fileAppender);
			dumpInfoThroughLogger(nameLogger, "nameLogger");
			
			
			Logger classLogger = Logger.getLogger(this.getClass());
			classLogger.setLevel(Level.ERROR);
			dumpInfoThroughLogger(classLogger, "classLogger");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
	
	@Test
	public void testAppender(){
		assertEquals(1, 1);
	}
	
	@Test
	public void testLayout(){
		assertEquals(1, 1);
	}
	
	private void dumpInfoThroughLogger(Logger logger, String strInfo){
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
        String strNow = df.format(new Date());
		logger.fatal(strNow);
		
		logger.debug(strInfo + " -- debug message");
		logger.info(strInfo + " -- info message");
		logger.warn(strInfo + " -- warn message");
		logger.error(strInfo + " -- error message");
		logger.fatal(strInfo + " -- fatal message");
	}
}
