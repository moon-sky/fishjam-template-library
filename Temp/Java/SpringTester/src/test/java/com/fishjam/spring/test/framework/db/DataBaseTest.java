package com.fishjam.spring.test.framework.db;

import java.sql.ResultSet;
import java.sql.SQLException;

import junit.framework.TestCase;

import org.junit.Test;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcDaoSupport;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;

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
 *   1.通过JDBC驱动程序定义
 *   2.通过JNDI查找 -- 数据源可以在应用程序之外进行管理，更改时不用重新编译，支持热切换
 *   3.连接池(实际运行环境中建议使用)
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
 *    
 * RowMapper -- 把数据映射为一个域对象
 * 
 * 数据访问模版，封装JDBC的样板式代码，只需将其配置为Spring中的Bean并织入到应用程序的DAO中
 * 将数据访问过程中固定的和可变的部分明确划分为两种不同的类：
 *   模版 -- 处理 事务控制、资源管理、异常处理 等
 *   回调 -- 应用程序相关的数据访问，如 创建语句、绑定参数、整理结果集 等
 *   针对不同的持久化平台，Spring提供了多个可选的模版 和 DAO支持类(应用程序的DAO可以继承自这些支持类)：
 *      HibernateTemplate -- Hibernate，可以分为 2、3 等多个版本
 *      JdbcTemplate | JdbcDaoSupport -- 直接使用JDBC
 *      JdoTemplate | JdoDaoSupport -- Java数据对象(Java Data Object) 实现
 *      JpaTemplate | JpaDaoSupport-- Java持久化API的实体管理器
 *      NamedParameterJdbcTemplate | NamedParameterJdbcDaoSupport -- 支持命名参数的JDBC连接
 *      SimpleJdbcTemplate | SimpleJdbcDaoSupport-- 通过Java5简化后的JDBC连接
 *      SqlMapClientTemplate | SqlMapClientDaoSupport -- iBATIS SqlMap 客户端
 * 
 *      
***********************************************************************************************************************************************/

//@RunWith(SpringJUnit4ClassRunner.class)
//@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class DataBaseTest extends TestCase { //AbstractJUnit4SpringContextTests {
	
	
	
	//@Autowired
	//private Student student;
	
	@Test
	public void testJdbcTemplate(){
		//JdbcTemplate template; 
		//Student stu = getStudentById(999);
		//assertNotNull(stu);
		assertEquals(1, 1);
	}
	
	//*
	JdbcTemplate jdbcTemplate;
	private Student getStudentById(int id) {
		return jdbcTemplate.queryForObject(
				"select id, firstname, lastname from student where id=?",
				new RowMapper<Student>() {
					public Student mapRow(ResultSet rs, int rowNum)
							throws SQLException {
						Student student = new Student(); // 将结果映射为对象
						student.setId(rs.getInt("id"));
						return student;
					}
				}, id); // 指定查询参数
	}
	//*/
	
	@Test
	public void testJDBC() {
		/*
		int id = 0;
		DataSource dataSource = null;  //getDataSource();

		Connection conn = null;
		PreparedStatement stmt = null;
		ResultSet rSet = null;
		try {
			conn = dataSource.getConnection();
			stmt = conn.prepareStatement("select id, firstname, lastname from employee where id=?");
			stmt.setLong(1, id);
			rSet = stmt.executeQuery();
			while (rSet.next()) {
				//employee = new Employee();
				//employee.setId(rSet.getLong("id"));
			}
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
		//fail("Not yet implemented");
		*/
	}
}
