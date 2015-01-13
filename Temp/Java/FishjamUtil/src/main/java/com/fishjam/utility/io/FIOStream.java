package com.fishjam.utility.io;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;
import java.security.Timestamp;
import java.sql.Date;
import java.sql.Time;
import java.text.SimpleDateFormat;
import java.util.zip.ZipOutputStream;

import javax.naming.directory.SearchControls;

import junit.framework.TestCase;

import org.junit.Test;

/***********************************************************************************************************************************************
* TODO:
*    [已读] java中的io系统详解 -- http://blog.csdn.net/ilibaba/article/details/3955799
*    http://blog.csdn.net/ilibaba/article/category/506698
*    
* 最佳实践
*   1.使用 new BufferedReader(new InputStreamReader(xxx, "utf-8")) -- 进行带缓冲的字节流读取=>转换成字符
*     使用 new BufferedInputStream(xxx) -- 进行带缓冲的字节流读取
***********************************************************************************************************************************************/

 
/***********************************************************************************************************************************************
 * 两种不同类型的 stream 之间的转换 (字节流 <=> 字符流) , 根据指定的编码方式(charset)进行转换 
 *    InputStreamReader :  读入字节流 ==> 生成字符流
 *    OutputStreamWriter:  读入字符流 ==> 生成字节流
 *    
 * 字节流(二进制) -- 处理单元为 1 个字节，操作字节和字节数组
 * <Closeable>
 * +-InputStream
 * +-+-ByteArrayInputStream -- 把内存中的一个缓冲区( 如 byte[] )作为 InputStream 使用
 * +-+-FilterInputStream -- 通过装饰器模式将InputStream封装至内部的一个成员变量
 * +-+-+-BufferedInputStream -- 带缓冲，其中的 read1 是非贪婪模式，能读多少读多少； read 是贪婪模式，没有读到足够多的数据就不返回(除非到了流的末尾).
 * +-+-+-DataInputStream
 * +-+-+-InflaterInputStream --
 * +-+-+-+-ZipInputStream
 * +-+-+-+-GZIPInputStream
 * +-+-+-LineNumberInputStream  -- 已被废弃，不再推荐使用
 * +-+-+-PushbackInputStream -- 
 * +-+-FileInputStream -- 以文件作为 InputStream，实现对文件的读取操作
 * +-+-ObjectInputStream -- 对象输入
 * +-+-PipedInputStream -- 实现了 pipe 的概念，主要在线程中使用。一个线程写入数据，另一个线程读取数据
 * +-+-SequenceInputStream -- 把多个 InputStream 合并为一个 InputStream。每个输入流依次被读取，直到到达该流的末尾，然后关闭该流并自动切换到下一个继续读取。
 *                                            TODO: 构造函数中使用枚举值初始化?
 * +-+-StringBufferInputStream(已废弃, 不能将字符正确的转换为字节, 推荐 StringReader) -- 把一个 String 对象作为 InputStream
 *---------------------------------------------------------------------------------------------------------------------------------------------
 * <Closeable> | <Flushable>
 * +-OutputStream
 * +-+-ByteArrayOutputStream -- 写入内存中的缓冲区(byte[])，实现一个以字节数组形式写入数据的输出流，数据写入缓冲区时，自动扩大(TODO: 会重新分配并拷贝?)
 *                                               可以通过 toString(charSet) 方法将缓冲区字节内容转换为字符串
 * +-+-FilterOutputStream
 * +-+-+-BufferedOutputStream -- 带缓冲(类似 BufferedInputStream )
 * +-+-+-CheckedOutputStream
 * +-+-+-DataOutputStream
 * +-+-+-DeflaterOutputStream
 * +-+-+-+-ZipOutputStream
 * +-+-+-+-GZIPOutputStream
 * +-+-+-PrintStream -- TODO: 可以格式化输出?
 * +-+-FileOutputStream -- 文件输出流，向 File 或 FileDescriptor 输出数据
 * +-+-PipedOutputStream -- 管道输出流， 是通讯管道的发送端
 * +-+-ObjectOutputStream -- 对象输出
 ***********************************************************************************************************************************************
 * 字符流(文本格式) --  2 个字节的 Unicode 字符，分别操作字符、字符数组或字符串。Java内部使用Unicode 编码存储字符，
 *    字符流处理类负责将外部的其他编码的字符流 和 Java内的 Unicode 字符流 之间进行转换。
 * <Closeable> | <Readable>
 * +-Reader
 * +-+-BufferedReader -- 带缓冲
 * +-+-+-LineNumberReader -- 
 * +-+-CharArrayReader -- 从字符数组中读取(char[])
 * +-+-FilterReader -- 
 * +-+-+-PushbackReader -- 
 * +-+-InputStreamReader -- 指定charset，处理 字节流 => 字符流 的转换
 * +-+-+-FileReader -- 
 * +-+-PipedReader -- 
 * +-+-StringReader -- 其源为一个字符串的字符流
 *----------------------------------------------------------------------------------------------------------------------------------------------
 * <Closeable> | <Flushable> | <Appendable>
 * +-Writer
 * +-+-BufferedWriter -- 带缓冲
 * +-+-CharArrayWriter -- 写到字符数组中(char[])
 * +-+-FilterWriter --  
 * +-+-OutputStreamWriter -- 指定charset，处理 字符流 => 字节流 的转换
 * +-+-+-FileWriter -- 
 * +-+-PipedWriter
 * +-+-PrintWriter -- TODO: 可以格式化输出?
 * +-+-StringWriter
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
* IO使用Stream(流)的抽象概念，将实际I/O设备中处理数据的细节动作隐藏起来，通过叠合多层
* 对象的形式获得想要的功能。通常使用完毕后需要使用close关闭文件
* 
* Reader和Writer提供了"兼容于Unicode并基于字符的I/O"，主要是为了国际化(旧的I/OStream
*   继承体系仅支持8Bit Byte Stream)。通过 InputStreamReader 和 OutputStream 进行转换。
* 
* 具备缓冲功能的输入文件：BufferedReader(FileInputReader) -- readLine到文件尾时返回null;
*   BufferedReader(InputStreamReader(System.in)) -- 缓冲(转换(标准输入))
* 输出至文件：PrintWriter(BufferedWriter(FileWriter)) -- 格式化(缓冲(文件))
* 
* Java可以通过static函数对标准输入、标准输出、标准错误输出等I/O Streams进行重定向
*   System.setIn(InputStream)
*   System.setOut(PrintStream)
*   System.setErr(PrintStream)
*   
* Java提供了压缩/解压缩 Streams 的类
*   压缩：DeflaterOutputStream <= ZipOutputStream/GZIPOutputStream
*   解压：InflaterInputStream  <= ZipInputStream/GZIPInputStream/ZipFile
*   CheckSum:Adler32(较快)，CRC32(较慢但比较精确)
*     ZipOutputStream(new BufferedOutputStream(new CheckedOutputStream(
*       new FileOutputStream("test.zip"),new Adler32())));
*       
* JAR--将某个applet所需的所有文件并在单一JAR文件内，浏览器只需向服务器请求一次就好，而且传输速度快。
*   具有内部文件的一份清单(manifest)
*   
* 对象序列化(Serialization)将实现了 Serializable 接口的对象的全部数据自动转换为连续bytes数据，
*   这些数据可通过网络传输或保存到磁盘，之后可被还原为原先的对象状态。注意：通过序列化
*   生成对象实例时，不会调用任何(包括缺省)构造函数，整个对象的状态全都通过InputStream
*   所得位数据进行回复--和MFC不同。可以使用关键字transient关闭指定成员变量不要序列化
*   ObjectOutputStream(new FileOutputStream).writeObject。
*   如果要对可序列化的部分进行控制（如安全问题），应该实现Externalizable接口，
*   并override其中的writeExternal和readExternal方法，注意：会调用缺省构造，然后通过
*   readExternal回复，并且通常需要调用基类的同名方法--和MFC相同
*   只要将所有东西写入单一stream，便可回复原先的对象网络，并且不会额外复制任何对象，
* 
* 持久化技术：
*    1.序列化(Serialization)
*    2.JDBC
*    3.ORM(Object-Relational-Mapping) -- 使用XML文件定义Java对象与关系数据库之间的映射
*    4.ODB() -- 对象数据库
*    5.JDO -- Java数据对象
*    6.EJB -- 
*    7.JPA -- 使用注解定义Java对象与关系数据库之间的映射，需要 JBoss Application Server + 
* 
***********************************************************************************************************************************************/

public class FIOStream extends TestCase
{
	class SerializablePerson implements Serializable{
		private String name;
		private Timestamp birthday;
		
		public String getName() {
			return name;
		}
		public void setName(String name) {
			this.name = name;
		}
		public Timestamp getBirthday() {
			return birthday;
		}
		public void setBirthday(Timestamp birthday) {
			this.birthday = birthday;
		}
	}
	
	@Test
	public void testSerializable(){
		SerializablePerson person = new SerializablePerson();
		assertEquals(1, 1);
	}

}
