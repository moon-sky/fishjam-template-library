package com.fishjam.springtester.controller;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.Controller;

//实现Controller接口
public class HelloWorldController implements Controller {
	
	public ModelAndView handleRequest(HttpServletRequest arg0,
			HttpServletResponse arg1) throws Exception {
		ModelAndView mv = new ModelAndView();
		mv.addObject("message", "你好，WELCOME  Spring MVC!");
		mv.setViewName("hello");
		return mv;
	}
}

