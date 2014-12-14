package com.fishjam.test.support;

import java.io.Serializable;

public class Student implements Serializable{
	private static final long serialVersionUID = -6060126844458076304L;
	
	public final static String SEX_MALE = "Male";
	public final static String SEX_FEMAIL = "Female";
	
	public Student(){
		
	}
	
	public Student(String name, String sex){
		this.name = name;
		this.sex = sex;
	}

	
	private String name;
	private String sex;

	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
}
