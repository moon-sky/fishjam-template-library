package com.fishjam.springtester.domain;

import java.sql.Timestamp;
import java.util.Iterator;

public class Student {
	public static final String SEX_MALE = "male";
	public static final String SEX_FEMALE = "female";
	
	public Student(){
		
	}
	public Student(int id){
		this.id = id;
	}
	
	private int id;
	private String name;
	private String sex;
	private Timestamp birthday;
	private PersonAction[] 	actions;
	
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
		return String.format("Student[id=%d,name:%s,birthday:%s, sex:%s, actions[size=%d]", 
				id, name, birthday != null ? birthday.toString() : "null",  sex, 
				actions != null ? actions.length : 0 );
	}
	
	public PersonAction[] getActions() {
		return actions;
	}
	public void setActions(PersonAction[] actions) {
		this.actions = actions;
	}
	public void DoAction() {
		if (actions != null) {
			for (PersonAction action : actions) {
				action.Do();
			}
		}
	}
}
