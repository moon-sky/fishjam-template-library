package com.java.test;

/***********************************************************************************************************************************************
 * FileOutputStream
 * FileInputStream
**********************************************************************************************************************************************/

import static org.junit.Assert.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.junit.Test;

public class IoTest {
	
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
