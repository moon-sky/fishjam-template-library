package com.java.tester;

import static org.junit.Assert.*;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import javax.sql.DataSource;
import javax.swing.tree.RowMapper;
import javax.swing.tree.TreePath;
import org.junit.Test;

/***************************************************************************************************************************
 * RowMapper -- 把数据映射为一个域对象
 * 
 *
 ****************************************************************************************************************************/




public class DataBaseTester {

	@Test
	public void testJDBC() {
		/*
		int id = 0;
		DataSource dataSource = null;  //getDataSource();

		Connection conn = null;
		PreparedStatement stmt = null;
		ResultSet rSet = null;
		try {
			conn = dataSource.getConnection();
			stmt = conn.prepareStatement("select id, firstname, lastname from employee where id=?");
			stmt.setLong(1, id);
			rSet = stmt.executeQuery();
			while (rSet.next()) {
				//employee = new Employee();
				//employee.setId(rSet.getLong("id"));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}finally{
			try {
				if (rSet != null) {
				rSet.close();
				}
				if (stmt != null) {
					stmt.close();
				}
				if (conn != null) {
					conn.close();
				}
			}catch (SQLException e) {
				e.printStackTrace();
			}
		}
		//fail("Not yet implemented");
		*/
	}

	/*
	private Employee getEmployeeById(long id){
		return jdbcTemplate.queryForObject("select id, firstname, lastname from employee where id=?",
				new RowMapper<Employee>() {
					public Employee mapRow(ResultSet rs, int rowNum) throws SQLException{
						Employee employee = new Emplyee();		//将结果映射为对象
						employee.setId(rs.getLong("id"));
						return employee;
					}
					//@Override
					//public int[] getRowsForPaths(TreePath[] path) {
					//	// TODO Auto-generated method stub
					//	return null;
					//}
				}, id);		//指定查询参数
	}
	*/
	@Test
	public void testJdbcTemplate(){
		//getEmployeeById(100);
	}
	

}
