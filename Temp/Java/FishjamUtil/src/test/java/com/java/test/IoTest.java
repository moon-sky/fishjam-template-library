package com.java.test;

/***********************************************************************************************************************************************
 * FileOutputStream
 * FileInputStream
 * ObjectInputStream/ObjectOutputStream -- 读写的对象必须实现Serializable接口，其中的
 *   transient和static类型成员变量不会被读取和写入
**********************************************************************************************************************************************/

import static org.junit.Assert.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import com.fishjam.test.support.Student;

import org.junit.Test;

public class IoTest {
	
		@Test
	public void testObjectStream(){
		try {
			final String TEST_FILE_NAME = "objectStream.dat";
			
			FileOutputStream foStream = new FileOutputStream(TEST_FILE_NAME);
			ObjectOutputStream ooStream = new ObjectOutputStream(foStream);
			ooStream.writeObject(new Student("fishjam", Student.SEX_MALE));
			foStream.close();
			ooStream.close();
			
			FileInputStream fiStream = new FileInputStream(TEST_FILE_NAME);
			ObjectInputStream oiStream = new ObjectInputStream(fiStream);
			Object objRead = oiStream.readObject();
			fiStream.close();
			oiStream.close();
			
			//System.out.println("objRead Class=" + objRead.getClass().getName());
			assertEquals("com.fishjam.test.support.Student", objRead.getClass().getName());
			
			
			Student stu = (Student)objRead;
			assertEquals("fishjam", stu.getName());
			assertEquals(Student.SEX_MALE, stu.getSex());

		} catch (Exception e) {
			e.printStackTrace();
		}
		
		assertEquals(1, 1);
	}
	
	@Test
	public void testKeyboardInOut(){
		assertTrue(true);
		//获取键盘输入并进行回显
		
		/*
		BufferedReader kbReader = new BufferedReader(new InputStreamReader(System.in));
		String inputString = null;
		try {
			while ( (inputString = kbReader.readLine()) != null) {		//每当在键盘上输入一行内容并回车确认，输入的内容会被读取
				System.out.println("UserInput: " + inputString);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		//*/
	}
}
