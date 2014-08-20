package com.fishjam.spring.test.framework;

import java.util.Hashtable;

import javax.naming.InitialContext;
import javax.naming.NamingException;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class JndiTest extends AbstractJUnit4SpringContextTests {
	
	@Test
	public void testInitialContext(){
		try {
			InitialContext initialContext = new InitialContext();
			System.out.println("NameInNamespace=" +  initialContext.getNameInNamespace());
			
			Hashtable<?, ?> environments =  initialContext.getEnvironment();
			System.out.println(environments.toString());
		} catch (NamingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}