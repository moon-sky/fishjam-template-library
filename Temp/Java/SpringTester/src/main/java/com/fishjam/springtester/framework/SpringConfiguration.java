package com.fishjam.springtester.framework;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.fishjam.springtester.domain.PersonAction;
import com.fishjam.springtester.domain.Student;
import com.fishjam.springtester.domain.StudyAction;

/***********************************************************************************************************************************************
 * 定义配置类，等价于XML中的<beans>配置，但仍然需要先在XML中通过 <context:component-scan>启用
 *    该类中会包含一个或多个 SpringBean 的定义 -- 使用 @Bean 注解标注的方法
 * 相对于XML中声明来说，其优点是可以进行编译器检查
 **********************************************************************************************************************************************/

@Configuration
public class SpringConfiguration {
	
	@Bean	//该注解告诉Spring，本方法将返回一个对象，该对象将被注册为Spring应用上下文中的一个Bean, 方法名即为Bean的ID
	public Student beanFromJava(){
		Student student = new Student();
		student.setId(1);
		student.setName("fish");
		PersonAction actions[] = { studyAction() } ;		//引用 studyAction 对应的Bean，注意：多次调用 studyAction()时不会每次都new一个实例(会缓存)
		student.setActions(actions);
		student.setSex(Student.SEX_MALE);
		return student;
	}
	
	@Bean
	public PersonAction studyAction(){
		PersonAction action = new StudyAction();
		return action;
	}
}
