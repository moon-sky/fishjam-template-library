package com.fishjam.spring.test.framework;

import static org.mockito.Mockito.*;
import org.junit.Test;

import com.fishjam.springtester.domain.PersonAction;
import com.fishjam.springtester.domain.Student;
import junit.framework.TestCase;

/*********************************************************************************************************
 * Mock 对象框架(Mockito)
 *    import static org.mockito.Mockito.*
 *  
 * 常见mock的方法
 *   when(xxx).thenReturn -- 
 *   times -- 
*********************************************************************************************************/


public class MockitoTest  extends TestCase {
	
	@Test
	public void testMockitoDemo(){
		PersonAction[] actions = new PersonAction[1];
		actions[0] = mock(PersonAction.class);		//创建PersonAction接口的mock实现
		
		Student student = new Student();
		student.setActions(actions);		//注入mock的Action

		//当调用过student.DoAction 后，要求Mockito框架验证PersonAction的mock实现的Do()方法仅被调用了一次
		student.DoAction();
		verify(actions[0], times(1)).Do();	
	}

	@Test
	public void testMockitReturn(){
		Student mockStudent = mock(Student.class);
		//当调用 Student.getName 时，指定返回 "fishjam" 的字符串
		when(mockStudent.getName()).thenReturn("fishjam");
		
		assertEquals(mockStudent.getName(), "fishjam");
	}
}
