package com.fishjam.spring.test.framework.db;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

/**************************************************************************************************************************************
 * http://www.cnblogs.com/ycxyyzw/archive/2012/10/13/2722567.html
 * 
 * iBatis(2010改名为MyBatis，并在GoogleCode托管) -- 基于SQL映射支持Java(SQL Maps)和·NET(DAO)的持久层框架， 类似 Hibernate(全自动映射)，
 *   但程序员需要编写具体的SQL，然后通过映射配置文件，将SQL所需的参数，以及返回的结果字段映射到指定 POJO。
 *   ibatis 以 SQL开发的工作量和数据库移植性上的让步，为系统设计提供了更大的自由空间。
 * 
 * 特点：
 *   1.封装了绝大多数的JDBC样板代码，使得开发者只需关注SQL本身。
 *   
 * 配置:
 *   sqlMapConfig.xml(iBatis)/Configuration.xml(MyBatis) -- 全局配置文件, 指定数据源、事务属性以及其他一些参数配置信息
 *     <Mapper><Session><Configuration>
 *   映射文件 -- 指定数据库表和程序之间的映射信息
 *
 * 简单的SQL语句可以用 @Select 直接写在Java源码里
 * 较复杂的SQL语句需要写在 .xml 配置文件里(这样以后更改了 SQL语句后，就不需要去改java代码了 )
 * 		<typeAlias alias="Student" type="com.iflytek.entity.Student" />  //
 * 		<select id="selectAllStudent" resultClass="Student">  //id表示select里的sql语句，resultClass表示返回结果的类型
 *			select * from 	tbl_student
 *		</select>
 * 
 * 用法：
 *   1.java中: 若返回int，则 ???
 *     @Repository public interface XxxDao{ 
 *        @Insert("insert into Xxx values( #{param1.property1} )") int addFuncName(@Param("param1") 参数类型 param1);
 *        @Update("delete from Xxx where param=#{param1.property1}") int delFunName(@Param("param1") 参数类型 param1);
 *        @Update("update Xxx set Yyy=#{param1.property1} where ...") int updateFunName(@Param("param1") 参数类型 param1);
 *        @Select("select * from Xxx where param=#{param1.property1}") list<返回值类型> selectFunName(@Param("param1") 参数类型 param1);
 *        
 * 
 * 辅助类库
 *   mybatis-spring -- 帮助将MyBatis代码和Spring进行无缝整合，Spring会自动加载必要的MyBatis工厂和session类，并控制事务.
 *   
 * 类和接口
 *   MapperFactoryBean -- 控制SqlSession创建和关闭它
 *   DataAccessException -- MyBatis中的异常类
 *   SqlSessionFactoryBean -- 
 *************************************************************************************************************************************/

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class IbatisTest extends AbstractJUnit4SpringContextTests {
	org.mybatis.spring.SqlSessionFactoryBean bean;
	
	@Test
	public void testIbatis(){
		
	}
}

