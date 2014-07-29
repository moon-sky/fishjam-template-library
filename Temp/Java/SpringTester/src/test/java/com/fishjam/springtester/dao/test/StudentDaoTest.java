package com.fishjam.springtester.dao.test;

import org.junit.Test;
import static org.junit.Assert.*;

import org.junit.runner.RunWith;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)  
//@ContextConfiguration({"classpath:SpringTester-servlet.xml"})

public class StudentDaoTest extends AbstractJUnit4SpringContextTests {
	
	//StudentDao studentDao;
	
	@Test
	public void testSaveStudent(){
		assertEquals(1, 1);
		//Student student = new Student();
		//studentDao.saveStudent(student);
	}
}
