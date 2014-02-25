package com.fishjam.storeserver.study;

/**************************************************************************************************************************************
 * Android sqlite数据库操作通用框架AHibernate(一)-CRUD示例和使用步骤
 *   http://blog.csdn.net/lk_blog/article/details/7455992
 * 
 * Hibernate -- 开源的对象关系映射及持久化框架，对JDBC进行了非常轻量级的对象封装，可以使用对象编程思维来操纵数据库。
 *   可以应用在任何使用JDBC的场合。甚至可以在应用EJB的J2EE架构中取代CMP，完成数据持久化的重任。
 *   使用 Java 反射机制来实现透明性
 *   使用了J2EE架构中的：JDBC、JTA、JNDI 等技术
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
 * ORM(对象-关系映射) -- 完成对象数据到关系数据映射的机制, 如 Java中的User类 <==> 数据库中的Tbl_User表
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
* 常见数据库
* sqlite是c写的，没有官方的操作sqlite的java api，如果要在Java中使用Sqlite的话，有几种方式：
*     1.SQLite JDBC Driver(https://bitbucket.org/xerial/sqlite-jdbc)下载(如 sqlite-jdbc-3.7.2.jar)
*       纯Java实现，方便，但效率较低
*     2.SQLite Java Wrapper/JDBC Driver( http://www.ch-werner.de/javasqlite/)， 需要本地库
*   支持内存数据库:  Connection connection = DriverManager.getConnection("jdbc:sqlite::memory:");
**************************************************************************************************************************************/

public class HibernateStudy {

	public testSqliteDemo(){
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
