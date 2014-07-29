package com.fishjam.springtester.service;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.fishjam.springtester.dao.StudentDao;
import com.fishjam.springtester.vo.Student;

@Service
public class StudentService {
	//private static final Log log = LogFactory.getLog(StudentService.class); 
	
	//@Autowired  //自动注入
	private StudentDao	studentDao;
	
	public void insert(Student student){
		//log.debug("insert " + student.getId());
		
		studentDao.saveStudent(student);
	}
	
	//public Student getStudentById(int id) { 
	//	 return studentDao.getStudentById(id); 
	//} 
}
