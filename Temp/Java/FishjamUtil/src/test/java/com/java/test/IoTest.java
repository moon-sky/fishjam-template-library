package com.java.test;

/***********************************************************************************************************************************************
 * 数据流类型:
 *   字节流:  
 *   字符流: BufferedReader -- 字符包装流，必须建立在另一个字符流的基础上。每次读取String对象
 *   对象流: ObjectInputStream/ObjectOutputStream -- 读写的对象必须实现Serializable接口，
 *            其中的transient和static类型成员变量不会被读取和写入
 *
 * 转换流(TODO?是否有这种概念?)
 *   InputStreamReader -- 字节流 => 字符流, 例子: BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
 *   
 * FileOutputStream
 * FileInputStream
 * 
 * 
 * Scanner(1.5以后,推荐使用) -- 基于正则表达式的文本扫描器，可从文件、输入流、字符串中解析出基本类型值和字符串值，通过System.in可以方便的获取用户的键盘输入。
 *   用法: hasNextXxx() + nextXxx()。默认时使用空白作为多个输入项之间的分隔符(通过 useDelimiter 设置)
**********************************************************************************************************************************************/

import static org.junit.Assert.*;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Scanner;

import com.fishjam.test.Student;

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
			assertEquals("com.fishjam.test.Student", objRead.getClass().getName());
			
			
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
	
	@Test
	public void testScanner(){
		assertTrue(true);
		
		/*
		Scanner scanner = new Scanner(System.in);
		scanner.useDelimiter("\n");		//使用回车作为分隔符
		while(scanner.hasNextLine()){
			String strInput = scanner.nextLine();
			System.out.println("Input[" + strInput.length() + "]:" + strInput);
			if(strInput.length() == 1){
				break;
			}
		}
		//*/
	}
}
