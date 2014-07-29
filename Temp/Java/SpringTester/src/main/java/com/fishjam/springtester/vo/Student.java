package com.fishjam.springtester.vo;

import java.sql.Timestamp;

public class Student {
	public static final String SEX_MALE = "male";
	public static final String SEX_FEMALE = "female";
	
	private int id;
	private String name;
	private String sex;
	private Timestamp birthday;
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
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
	public Timestamp getBirthday() {
		return birthday;
	}
	public void setBirthday(Timestamp birthday) {
		this.birthday = birthday;
	}
	@Override
	public String toString() {
		return String.format("Student[id=%d,name:%s,birthday:%d,sex:%s]", 
				id, name, birthday.toString(), sex);
	}
}
