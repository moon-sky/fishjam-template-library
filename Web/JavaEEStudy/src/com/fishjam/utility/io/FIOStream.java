package com.fishjam.utility.io;

import java.io.Serializable;

/************************************************************************************************************
* IO使用Stream(流)的抽象概念，将实际I/O设备中处理数据的细节动作隐藏起来，通过叠合多层
* 对象的形式获得想要的功能。通常使用完毕后需要使用close关闭文件
* 
* Reader和Writer提供了"兼容于Unicode并基于字符的I/O"，主要是为了国际化(旧的I/OStream
*   继承体系仅支持8Bit Byte Stream)。通过InputStreamReader 和 OutputStream 进行转换。
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
* 对象序列化(Serialization)将实现出 Serializable 接口的对象的全部数据自动转换为连续bytes数据，
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
************************************************************************************************************/

public class FIOStream
{
    
}
