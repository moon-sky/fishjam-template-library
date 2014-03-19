package com.fishjam.utility.dumper.tester;

import java.util.ArrayList;

import junit.framework.TestCase;

import com.fishjam.utility.dumper.BaseInfoDumper;

public class BaseInfoDumperTester extends TestCase {

	public void testCollectionToString() {
		ArrayList <Integer> IntArray = new ArrayList<>();
		assertEquals("", BaseInfoDumper.CollectionToString(IntArray));
		
		for (int i = 0; i < 10; i++) {
			IntArray.add(i);
		}
		
		assertEquals("0,1,2,3,4,5,6,7,8,9", BaseInfoDumper.CollectionToString(IntArray));
		assertEquals("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]", IntArray.toString());
	}

	public void testEnumerationToString() {
		assertEquals(1, 1);
	}

	public void testListToString() {
		assertEquals(1, 1);
	}

	public void testMapToString() {
		assertEquals(1, 1);
	}

}
