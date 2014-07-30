package com.fishjam.springtester.controller;


import java.sql.Timestamp;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.Controller;

import com.fishjam.springtester.domain.Student;
import com.fishjam.springtester.service.StudentService;

//实现Controller接口
//@Controller
public class HelloWorldController implements Controller {
	
	@Resource
	private StudentService studentService;
	
	//@RequestMapping("/springtester/hello")
	public ModelAndView handleRequest(HttpServletRequest arg0,
			HttpServletResponse arg1) throws Exception {
		
		Student student = new Student();
		student.setId(1);
		student.setName("fishjam");
		student.setSex(Student.SEX_MALE);
		student.setBirthday(new Timestamp(1979, 3, 30, 12,0,0, 0));
		
		studentService.insert(student);
		
		ModelAndView mv = new ModelAndView();
		mv.addObject("message", "fishjam 你好，WELCOME  Spring MVC!");
		mv.setViewName("hello");
		return mv;
	}
}

