package com.fishjam.study.javaee;

//http://my.oschina.net/liangbo/blog?catalog=267685

/**************************************************************************************************************************************
 * TODO:
 *   1.对象类(如User)中可加入 XDoclet 注释(如 @hibernate.class table="users" ),
 *     然后通过 xdoclet ant task 自动生成对应的 user.hbm.xml 文件(参见"hibernate开发指南")
 *   2.使用Sqlite时的相对路径方式
 *     TODO: web中的话.方法很简单 Web.xml文件中.使用<context-param> 标签就可以了.
 *                用框架配置的话. 就使用${param-name}就可以得到值了.  
 *                如果是一般的servlet 只要用getInitParameter(param-name)就可以拿到了.
 *     
 * Android sqlite数据库操作通用框架AHibernate(一)-CRUD示例和使用步骤
 *   http://blog.csdn.net/lk_blog/article/details/7455992
 * 
 * 其他ORM方案(注意：都是小范围的方案)：
 *   JPA -- Sun 的
 *   TopLink -- Oracle 的
 *   OJB -- Apache 的
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Hibernate -- 开源的对象关系映射及持久化框架，对JDBC进行了非常轻量级的对象封装，可以将 POJO 映射成持久化类，然后使用对象编程思维来操纵数据库。
 *   可以应用在任何使用JDBC的场合。甚至可以在应用EJB的J2EE架构中取代CMP，完成数据持久化的重任。
 *   使用 Java 反射机制来实现透明性， 使用了J2EE架构中的：JDBC、JTA、JNDI 等技术
 *   
 * 框架结构：Session Bean <-> DAO <-> Hibernate <-> DB
 * 优点：
 *   1.对JDBC访问数据库的代码做了封装，大大简化了数据访问层繁琐的重复性代码
 *   2.是一个优秀的ORM实现。大大简化了DAO层的编码工作 ，使开发更对象化
 *   3.移植性好，支持各种数据库，如果换个数据库只要在配置文件中变换配置就可以了，不用改变hibernate代码
 *   4.支持透明持久化，因为hibernate操作的是纯粹的（pojo）java类，没有实现任何接口，没有侵入性
 *   5.可以用在任何JDBC可以使用的场合(即使手机上?)
 *   6.支持各种关系数据库，从一对一到多对多的各种复杂关系
 * 缺陷：
 *   1.限制您所使用的对象模型。(例如，一个持久性类不能映射到多个表)
 * 
 *
 * Hibernate运行在两种环境下：
 *  可管理环境 -- 这种环境可管理如下资源：池资源，数据库连接池，事务、安全定义等。一些典型的J2EE服务器（JBoss、Weblogic、WebSphere）已经实现了这些。
 *  不可管理环境 -- 只是提供了一些基本的功能，诸如像Jetty或Tomcat这样的servlet容器环境。
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * hibernate.cfg.xml 
 *   <mapping resource="com/xxx/User.hbm.xml"
 *
 * Dialect -- 方言。hibernate为了更好的适配各种关系数据库，针对每种数据库都指定了一个方言Dialect，用来把Java对象转换成关系数据库。
 *   本质是一个 从 org.hibernate.dialect.Dialect 继承的类，
 *
 *   官方目前还不支持使用 SQLite 数据库, 开源第三方dialect(https://code.google.com/p/hibernate-sqlite/)
 *     使用：<property name="dialect">com.applerao.hibernatesqlite.dialect.SQLiteDialect</property>
 *     Bug:1.不支持分页查询( where xxx limit xxx offset yyy )
 * 
 * 访问JNDI 指定的数据库: 
 *   Context ctx = new InitialContext(); 
 *   DataSource ds = (DataSource)ctx.lookup("java:comp/env/" + "自定义名字");
 *
 * 常用数据库连接配置，
 *   Access		sun.jdbc.odbc.JdbcOdbcDriver
 *   MySql		com.mysql.jdbc.Driver						jdbc:mysql://localhost:3306/javaee，		mysql-connector-java-5.1.7-bin.jar
 *   Oracle     oracle.jdbc.OracleDriver
 *   SQLite     org.sqlite.JDBC									jdbc:sqlite:c:/sqlitedemo.db					sqlite-jdbc-3.7.2.jar
 *   使用方式：
*       Class.forName("com.mysql.jdbc.Driver");
*       Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/javaee","root","root");	
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 常用接口
 *   Callback -- 
 *   Configuration -- 读取配置创建SessionFactory对象？
 *     buildSessionFactory()
 *   Criteria -- 允许创建并执行面向对象的标准化查询
 *   Query -- 方便地对数据库及持久对象进行查询。有两种表达方式：HQL语言或本地数据库的SQL语句。
 *            常被用来绑定查询参数、限制查询记录数量，并最终执行查询操作。
 *   Session -- 轻量级的类，创建和销毁都不会占用很多资源
 *     beginTransaction()
 *     getTransaction().commit()
 *   SessionFactory -- 工厂模式取得Session的实例，通常其实例在整个应用中共享。
 *     openSession()
 *   Transaction
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Annotation

**************************************************************************************************************************************/

/**************************************************************************************************************************************
* 常见数据库
* sqlite是c写的，没有官方的操作sqlite的java api，如果要在Java中使用Sqlite的话，有几种方式：
*     1.SQLite JDBC Driver(https://bitbucket.org/xerial/sqlite-jdbc)下载(如 sqlite-jdbc-3.7.2.jar)
*       纯Java实现，方便，但效率较低
*     2.SQLite Java Wrapper/JDBC Driver( http://www.ch-werner.de/javasqlite/)， 需要本地库
*   支持内存数据库:  Connection connection = DriverManager.getConnection("jdbc:sqlite::memory:");
*   支持资源型数据库(尚未测试通过, org\sqlite\Conn.java 中的实现发现): .getConnection("jdbc:sqlite::resource:xxx"); 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 使用 Hibernate 的示例:

@Entity		 							//映射实体， 普通的POJO通过该标注映射成为可持久化的类 (等价于实现Serializable？)
@Table(name="tbl_User", uniqueConstraints = {@UniqueConstraint(columnNames = {"name", "email"} )
//指定表的属性，如 (name,catalog,shema,uniqueConstraints 等), @UniqueConstraint 表示 不能同时存在完全相同的name和email值的记录
class UserTableInfo implements java.io.Serializable{					//定义表对应的实体类
	@Id										//标注主键
	@Column(name="user_id")	//该字段对应的列属性(如 name, unique="true", nullable="false", 等)
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private Integer id;
	
	
	@OneToMany(targetEntity=Item.class ,mappedBy="owner")
	@ManyToOne
	
	UserTableInfo(){						//@Entity 标注的类必须有一个无参构造方法
	}
	//各个字段的 get/set 方法
}
**************************************************************************************************************************************/

public class HibernateStudy {

	public void testSqliteDemo(){
		/*
		try {
			String fileName = "c:/SqliteDemo.db";  // Linux: /home/myuser/SqliteDemo.db
			Class.forName("org.sqlite.JDBC");
			// Create Connection Object to SQLite Database, If you want to only create a database in memory, exclude the fileName
			Connection conn = DriverManager.getConnection("jdbc:sqlite:" + fileName);
			// Create a Statement object for the database connection, dunno what this stuff does though.
			Statement stmt = conn.createStatement();
			
			//stmt.executeUpdate("drop table if exists tbl_User");
			//statement.executeUpdate("create table person (id integer, name string)");
			
			// Create a result set object for the statement
			ResultSet rs = stmt.executeQuery("SELECT * FROM tbl_User");
			
			// Iterate the result set, printing each column if the column was an int, we could do rs.getInt(column name here) as well, etc.
			while (rs.next()) {
				int id = rs.getInt("Id");					// Column 1
				String username = rs.getString("UserName"); // Column 2
				String password = rs.getString("Password"); // Column 3
				System.out.println("ID: " + id + " username: " + username + " password: " + password);
			}
			System.out.println("End loop table");
			// Close the connection
			conn.close();
		} catch (Exception e) {
			// Print some generic debug info
			System.out.println(e.getMessage());
			System.out.println(e.toString());
		}
		*/
	}

}
