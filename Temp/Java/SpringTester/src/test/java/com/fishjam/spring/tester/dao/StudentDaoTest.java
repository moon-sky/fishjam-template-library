package com.fishjam.spring.tester.dao;

import static org.junit.Assert.assertEquals;

import javax.annotation.Resource;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.dao.StudentDao;
import com.fishjam.springtester.domain.Student;

@RunWith(SpringJUnit4ClassRunner.class)  
//@ContextConfiguration(locations = {"classpath:SpringTester-servlet.xml"})
@ContextConfiguration({"/app*.xml", "Spring*.xml", "Context*.xml"})
public class StudentDaoTest extends AbstractJUnit4SpringContextTests {
	
	@Resource
	StudentDao studentDao;
	
	public void setStudentDao(StudentDao studentDao) {
		this.studentDao = studentDao;
	}

	@Test
	public void testSaveStudent(){
		Student student = new Student();
		student.setId(100);
		student.setName("fishjam");
		student.setSex(Student.SEX_MALE);
		
		studentDao.saveStudent(student);
		assertEquals(1, 0);
	}
}
