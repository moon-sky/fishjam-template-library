package com.fishjam.springtester.dao;

//数据访问层: 提供了基于 Spring Jdbc Template 实现的数据库访问方法

import java.util.HashMap;
import java.util.Map;

import org.springframework.jdbc.core.namedparam.NamedParameterJdbcDaoSupport;
import org.springframework.stereotype.Repository;

import com.fishjam.springtester.domain.Student;

@Repository
public class StudentDao extends NamedParameterJdbcDaoSupport{

	 public void saveStudent(Student student) { 
		 String sql = "insert into tbl_student(id,name,birthday,sex) " + 
				"values(:id,:name,:birthday,:sex)"; 
		 Map paramMap = new HashMap(); 
		 paramMap.put("id", student.getId()); 
		 paramMap.put("name", student.getName()); 
		 paramMap.put("birthday", student.getBirthday()); 
		 paramMap.put("sex",student.getSex()); 
		 getNamedParameterJdbcTemplate().update(sql, paramMap); 
	 } 
	
	 /*
	 public Student getAccountById(int id) { 
		 String sql = "select id,name,birthday,sex from tbl_student where id=:id"; 
		 Map paramMap = new HashMap(); 
		 paramMap.put("id", id); 
		 List<Student> matches = getNamedParameterJdbcTemplate().query(sql, 
				 paramMap,new ParameterizedRowMapper<Student>() { 
					 @Override 
					 public Student mapRow(ResultSet rs, int rowNum) 
							 throws SQLException { 
						 Student student = new Student(); 
						 student.setId(rs.getInt(1)); 
						 student.setName(rs.getString(2)); 
						 student.setBirthday(rs.getTimestamp(3)); 
						 student.setSex(rs.getString(4)); 
						 return student; 
					 } 
			
		 }); 
		 return matches.size()>0?matches.get(0):null; 
	 } 
	 */
}
