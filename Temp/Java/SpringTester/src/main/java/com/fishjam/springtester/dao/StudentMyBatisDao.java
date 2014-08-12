package com.fishjam.springtester.dao;

import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import com.fishjam.springtester.domain.Student;


/**************************************************************************************************************************************
 * 数据映射器 -- 必须是一个接口，通过注解的方式来指定SQL，也可以通过XML指定
**************************************************************************************************************************************/


public interface StudentMyBatisDao {

	@Select("Select * from student where id=#{id}")
	Student getStudent(@Param("id") int id);
}
