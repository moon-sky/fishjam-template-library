package com.fishjam.spring.tester.domain;

import static org.junit.Assert.*;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.domain.Student;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
//@ContextConfiguration(locations={"file:src/main/webapp/WEB-INF/applicationContext.xml"})
public class PersonBeanTest extends AbstractJUnit4SpringContextTests{
	
	@Autowired
	private Student	student;
	
	@Test
	public void testLoadBeanFromXml(){
		//assertNotNull(student);
		assertEquals(student.getId(), 999);
	}
}
