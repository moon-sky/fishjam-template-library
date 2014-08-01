package com.fishjam.spring.tester.framework;

/**********************************************************************************************************
 * RowMapper -- 把数据映射为一个域对象
 * JdbcTemplate -- 封装JDBC的样板式代码
**********************************************************************************************************/
import java.sql.ResultSet;
import java.sql.SQLException;

import junit.framework.TestCase;

import org.junit.Test;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import com.fishjam.springtester.domain.Student;

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
}
