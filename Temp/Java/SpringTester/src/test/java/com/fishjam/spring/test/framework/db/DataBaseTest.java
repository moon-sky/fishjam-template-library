package com.fishjam.spring.test.framework.db;

import static org.junit.Assert.assertEquals;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.HashMap;
import java.util.Map;

import javax.sql.DataSource;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.annotation.PersistenceExceptionTranslationPostProcessor;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.simple.ParameterizedRowMapper;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Transactional;

import com.fishjam.springtester.domain.Student;

/***********************************************************************************************************************************************
 * JDBC -- Java Database Connectivity 
 * JDBC通过驱动管理器(Driver Manager)动态维护所有数据库查询所需的所有驱动程序对象，
 *   使用Class.forName("类名全称")强迫装载驱动，并会自动注册到管理器中
 * 产生数据库URL("jdbc:子协议:数据库标识符")--如 jdbc:odbc:mydatabase
 *   或 jdbc:rmi://192.168.0.1:1099/jdbc:cloudscape:db -- 远程RMI使用DB
 *   DriverManager.getConnection("").createStatement().executeQuery("");//记住close
 *   
 * 配置数据源(Spring 提供了多种配置数据源Bean的方式)
 *   1.通过JDBC驱动定义 -- 最简单的配置方式，Spring提供了两种数据源对象供选择:
 *     a.DriverManagerDataSource -- 在每个连接请求时都会返回一个新建的连接，没有进行池化管理，性能消耗大
 *     b.SingleConnectionDataSource -- 每个连接请求时会返回同一个连接, 不适用于多线程的应用程序
 *   2.通过JNDI查找 -- 数据源可以在应用程序之外进行管理，更改时不用重新编译，支持热切换
 *      <jee:jndi-lookup> 检索JNDI中的数据源对象并应用于Bean中，其中 jndi-name 指定JNDI中资源的名称。如只设置了 jndi-name 属性，
 *      会根据指定的名称查找数据源，但如果应用程序运行在Java应用程序服务器中，需要设置 resource-ref 为true,这样会自动添加 java:comp/env/ 前缀
 *   3.连接池(实际运行环境中建议使用)， Spring没有提供数据源连接池的实现，但有其他的多种实现:
 *     a.dbcp -- Apache的 Jakarta Commons Database Connection Pooling， 缺点：没有自动回收空闲连接的功能
 *       BasicDataSource -- 最常用，易于在Spring中配置，类似于Spring自带的DriverManagerDataSource 
 *     b.c3p0 -- 开源的JDBC连接池，优点：有自动回收空闲连接的功能
 *     
 *   
 *    
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * 打开顺序: 
 * 关闭顺序: ResultSet -> PreparedStatement -> Connection -> DataSource ?
 * 
 * 常见DB相关的类   
 *   Connection
 *     createStatement() -- 创建执行环境，用于 SQL 中不带 "?" 参数的情况
 *     prepareStatement(sql) -- 准备select等语句的执行环境，用于SQL中带 "?" 参数的情况
 *   DataSource -- 一般可以通过 new InitialContext().lookup("java:comp/env/jdbc/xxxx") 从JNDI中获得
 *     getConnection
 *   DriverManager
 *     getConnection()
 *   PreparedStatement
 *     executeUpdate() -- 执行更新相关的语句
 *     executeQuery() -- 执行 select 相关的语句，返回 ResultSet
 *     setObject -- 替换SQL语句中的"?" 参数, 如 ("select * from user_table  where name= ?", username); 
 *       函数声明 insert(String sql, Object... args) { 	PreparedStatement pstmt = getConnection().prepareStatement(sql);  for{ 	pstmt.setObject( i + 1 , args[i]); }
 *   ResultSet -- query 后返回的结果集
 *     getMetaData() -- 获取
 *     next() -- 到下一条记录，如还有则返回true，之后可通过 getString 等方法获取数据，一般用法为 while(rs.next()) { rs.getString(1)... }
 *     updateString -- 更新字段值
 *     updateRow -- 更新一行
 *   ResultSetMetaData -- ?
 *     getColumnCount() -- 获取列数目
 *   Statement
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * Spring 提供了一组数据访问框架，继承了多种数据访问技术
***********************************************************************************************************************************************/
 
/***********************************************************************************************************************************************
 * Spring没有尝试去创建自己的ORM解决方案，而是对许多流行ORM框架(Hibernate,JPA, JDO, iBATIS 等)进行了集成。
 * Spring提供的持久化异常 -- 具有描述性且与特定的持久化框架无关, 都继承自 DataAccessException(非检查型异常，即不是必须捕获异常才能编译)，如(有很多):
 *    CannotAcquireLockException
 *    ConcurrencyFailureException
 *    InvalidDataAccessResourceUsageException
 *    TypeMismatchDataAccessException
 * 为了给不使用模版的DAO添加异常转换功能，需要在Spring应用上下文中添加一个 <bean class="org.springframework.dao.annotation.PersistenceExceptionTranslationPostProcessor" /> 的Bean
 *    是一个Bean的后置处理程序，会在所有拥有 @Repository 注解的类上添加一个通知器(advisor)，捕获任何平台相关的异常并以Spring
 *    的非检查型数据访问异常的形式重新抛出
 *    
 * RowMapper -- 把数据映射为一个域对象
 * 
 * 数据访问模版，封装JDBC的样板式代码(资源管理和异常处理)，只需将其配置为Spring中的Bean并织入到应用程序的DAO中
 * 将数据访问过程中固定的和可变的部分明确划分为两种不同的类：
 *   模版 -- 处理 事务控制、资源管理、异常处理 等
 *   回调 -- 应用程序相关的数据访问，如 创建语句、绑定参数、整理结果集 等
 *   针对不同的持久化平台，Spring提供了多个可选的模版 和 DAO支持类(应用程序的DAO可以继承自这些支持类,然后可使用 getXxxTemplate 等方法)：
 *      HibernateTemplate -- Hibernate，可以分为 2、3 等多个版本
 *      JdbcTemplate | JdbcDaoSupport -- 直接使用JDBC，支持最简单的JDBC数据库访问功能及简单的索引参数查询
 *      JdoTemplate | JdoDaoSupport -- Java数据对象(Java Data Object) 实现
 *      JpaTemplate | JpaDaoSupport-- Java持久化API的实体管理器
 *      (已合并到SimpleXxx)NamedParameterJdbcTemplate | NamedParameterJdbcDaoSupport -- 支持命名参数(而不是使用简单的索引参数)的JDBC连接
 *      SimpleJdbcTemplate | SimpleJdbcDaoSupport-- 利用Java5的一些特性(如自动装箱、泛型及可变参数列表等)的JDBC连接
 *      SqlMapClientTemplate | SqlMapClientDaoSupport -- iBATIS SqlMap 客户端
 * 
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * ORM框架的特性
 *   延迟加载(Lazy loading) -- 只获取需要的数据，避免大的开销
 *   预先抓取(Eager fetching) -- 与延迟加载相对，可以使用一个查询获取完整的关联对象，节省多次查询的成本
 *   级联(Cascading) -- 更改表中数据时，自动更改关联表的数据
 *   缓存
 *   分布式缓存 -- hibernate 支持
 *   
 * 
 * 集成持久化框架(Hibernate) 
 *   1.使用方式已被淘汰? -- Spring提供了 HibernateTemplate 模版类来抽象其持久化功能 -- 管理Session、
 *   2.最佳实践 -- 使用Hibernate提供的 上下文Session(ContextualSession)
 *     a.配置 SessionFactory(负责Session的打开、关闭和管理)
 *       指定持久化域对象是通过XML文件(LocalSessionFactoryBean)还是通过注解(AnnotationSessionFactoryBean)来进行配置
 *       <bean id="sessionFactory" class="org.springframework.orm.hibernate3.LocalSessionFactoryBean" > [dataSource], [mappingResources],[hibernateProperties]
 *       <bean id="sessionFactory" class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean"> [dataSource],[packagesToScan],[hibernateProperties]
 *       hibernateProperties -- 配置Hibernate如何进行操作的细节，即使用什么数据库的 Dialect 
 *       packagesToScan -- 告诉Spring扫描包的路径列表，其类使用 @Entrity 或 @MappedSuperclass 注解
 *    b. ? 直接使用 Hibernate 的 org.hibernate.SessionFactory, 可通过其 getCurrentSession 方法获取Session
***********************************************************************************************************************************************/
/***********************************************************************************************************************************************
 * 集成JPA(配置文件为 persistence.xml )
 *    JPA定义了两种类型的实体管理器( EntityManager ), JpaTemplate 隐藏了两者的处理细节：
 *    a.应用程序管理类型(Application-managed, createEntityManagerFactory) -- 想实体管理器工厂请求实体管理器时，工厂创建一个，程序要负责对齐进行控制，
 *      适合于不运行在Java EE容器中的独立应用程序。对应的 Spring工程Bean为: LocalEntityManagerFactoryBean 
 *    b.容器管理类型(Container-managed, createContainerEntityManagerFactory) -- 实体管理器由 JavaEE 容器创建和管理，
 *      通过注入或JNDI来获取，适合于 JavaEE 容器. 对应的 Spring工程Bean为: LocalContainerEntityManagerFactoryBean
 *      
 * Spring提供了多个JPA厂商适配器,在配置中通过 jpaVendorAdapter 属性指定：
 *   EclipseLinkJpaVendorAdapter
 *   HibernateJpaVendorAdapter
 *   OpenJpaVendorAdapter
 *   TopLinkJpaVendorAdapter
 * 
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
* Spring对事务的支持(tx 配置命名空间)
*   1.编码式 -- 通过回调机制实现， TransactionTemplate.execute( new TransactionCallback<Void>(){ ... }); 
*   2.声明式 -- 通过AOP框架实现，@Transactional，只能在方法级别声明事务边界，可以配置事务属性：
*     a.传播行为(propagation) -- 定义了客户端与被调用方法之间的事务边界, TransactionDefinition 中常量值(PROPAGATION_REQUIRES_NEW 等)
*     b.隔离级别(isolation) -- 定义了事务可能受其他并发事务影响的程度，锁定数据库中的记录或表，但可能导致性能问题，可选 ISOLATION_SERIALIZABLE(最安全但最慢) 等
*     c.是否只读(read-only) -- 如事务只进行读操作，数据库可进行特殊优化，需要是启动新事务的传播行为才有意义 
*     d.事务超时(timeout) -- 设置超时值，超过时间后自动回滚
*     e.回滚规则(rollback-for / no-rollback-for) -- 定义了哪些异常会导致事务回滚而哪些不会，默认：运行期异常时回滚，检查型异常不回滚
*  
* Spring 不直接管理事务，而是提供多种事务管理器，将对应的职责委托给JTA等持久化机制所提供的平台相关的事务实现 
*   DataSourceTransactionManager[dataSource] -- 用于Spring对JDBC抽象的支持，也可用于使用 iBATIS 进行持久化的场景，内部通过 Connection 来管理事务(commit + rollback)
*   HibernateTransactionManager[sessionFactory] -- 用于 Hibernate3 进行持久化
*   xxx.orm.jpa.JpaTransactionManager[entityManagerFactory] -- 用于JPA持久化
*   transaction.jta.JtaTransactionManger -- 需要分布式(XA)事务或没有其他的事务管理器满足需求 
*
* 定义事务
*   1.XML中定义(需要 aop 支持) -- 参见 DemoDatabase.xml 中的 <tx:advice> + <aop:config>
*   2.注解中定义 -- a.在 XML 中通过 <tx:annotation-driven/> 允许使用注解事务; b.使用 @Transactional 注解Bean或方法
***********************************************************************************************************************************************/


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoDatabase.xml"})
public class DataBaseTest extends AbstractJUnit4SpringContextTests {
	
	
	@Autowired
	private DataSource dataSource;
	
	@Autowired
	private JdbcTemplate	jdbcTemplate;
	//@Autowired
	//private Student student;
	
	@Test
	public void testJdbcTemplate(){
		//JdbcTemplate template; 
		//Student stu = getStudentById(999);
		//assertNotNull(stu);
		assertEquals(1, 1);
		try {
			CreateStudentTable();
			Student originalStudent = new Student();
			originalStudent.setName("fishjam");
			originalStudent.setSex(Student.SEX_MALE);
			originalStudent.setBirthday( new Timestamp(1979, 3, 30, 12, 0, 0, 0));
			int nNewId = InsertStudentIntoDB(originalStudent);
			originalStudent.setName( originalStudent.getName() + "[Change]");
			UpdateStudentInfo(originalStudent);
			
			Student queryStudent = QueryStudentById(nNewId);
			assertEquals(originalStudent.getName() + "[Change]", queryStudent.getName());
			assertEquals(originalStudent.getSex(), queryStudent.getSex());
		} finally{
			DropStudentTable();
		}
	}
	
	private void CreateStudentTable(){
		jdbcTemplate.update("create table student (id int not null, "
				+ "name varchar, "
				+ "sex varchar)"
				);
	}
	private int InsertStudentIntoDB(Student student){
		//使用参数索引的方式
		jdbcTemplate.update("insert into student(id, name, sex) values( ?, ?, ?)",
				null,
				student.getName(), 
				student.getSex()
				);
		//student.setId(queryForIdentity());
		return 0;
	}
	
	private int UpdateStudentInfo(Student student){
		//使用命名参数的方式 更新DB
		Map<String, Object> params = new HashMap<String, Object>();
		params.put("id", null);
		params.put("name", student.getName());
		params.put("sex", student.getSex());
		jdbcTemplate.update("update student set name = :name, sex = :sex where id=:id ", params); 
		return student.getId();
	}
	
	private Student QueryStudentById(int nId) {
			return jdbcTemplate.queryForObject(
					"select id, name, sex from student where id=?",	//参数1: 从数据库中查找数据的SQL
					new ParameterizedRowMapper<Student>(){		//参数2: RowMapper 实例，用来从ResultSet中提取值并构建域对象
						public Student mapRow(ResultSet rs, int rowNum)	//TODO: 如果是查询结果有多条?
								throws SQLException {
							Student student = new Student();	// 将结果映射为对象
							student.setId(rs.getInt(1));
							student.setName(rs.getString("name"));
							student.setSex(rs.getString(3));
							return student;
						}
					}, nId																	//参数3: 可变参数列表，列出了要绑定到查询上的索引参数值
				);
	}
	private void DropStudentTable() {
		jdbcTemplate.update("drop table if exists student");
	}

	@Test
	public void testJDBC() {
		//*
		int id = 0;
		//DataSource dataSource = null;  //getDataSource();

		Connection conn = null;
		PreparedStatement stmt = null;
		ResultSet rSet = null;
		try {
			conn = dataSource.getConnection();
			//conn.prepareStatement("create table employee ( id int NOT NULL")
			stmt = conn.prepareStatement("select id, firstname, lastname from employee where id=?");
			stmt.setLong(1, id);
			rSet = stmt.executeQuery();	 //stmt.execute();
			while (rSet.next()) {
				//employee = new Employee();
				//employee.setId(rSet.getLong("id"));
				//listResult.add(employee);
			}
			//return listResult;
		} catch (Exception e) {
			e.printStackTrace();
		}finally{
			try {
				if (rSet != null) {
				rSet.close();
				}
				if (stmt != null) {
					stmt.close();
				}
				if (conn != null) {
					conn.close();
				}
			}catch (SQLException e) {
				e.printStackTrace();
			}
		}
		//return null;
		//fail("Not yet implemented");
		//*/
	}
}
