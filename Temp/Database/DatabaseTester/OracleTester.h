#pragma once

/*****************************************************************************************************
* 执行 sql 命令
* sqlplus username/pwd@ServerName  [ < xxx.sql ]
*****************************************************************************************************/

/*******************************************************************************************************
* 性能优化：
*   8i与9i时代，使用 StatSpack 工具，人工分析监控数据，找到性能瓶颈并解决；
*   10i 时：运行 ADDM 报告，根据性能调整建议，修改一下初始参数，添加几个索引，或者根据SQL Tunning Advisor使用
*     SQL Profile调整SQL语句的执行计划，即可完成大部分性能优化。
*
*
*******************************************************************************************************/

/*******************************************************************************************************
* Oracle释放磁盘(Undo、Temp表)空间
*   1.查看各表空间名称：
*     select name from v$tablespace
*   2.查看某个表空间信息(大小)，确定释放哪个表：
*     select file_name,bytes/1024/1024 from dba_data_files where tablespace_name like 'UNDOTBS1';
*   3.查看是否有人正在使用回滚段(防止有用户正在使用)
*     select s.username, u.name from v$transaction t,v$rollstat r, v$rollname u, v$session s
* 　　where s.taddr=t.addr and  t.xidusn=r.usn and r.usn=u.usn order by s.username;
*   4.创建新的UNDO表空间，并设置自动扩展参数
*   5.create undo tablespace undotbs2 datafile 'D:\ORACLE\PRODUCT\10.1.0\ORADATA\ORCL\UNDOTBS02.DBF' 
*     size 10m reuse autoextend on next 100m maxsize unlimited;
*   6.动态更改spfile配置文件
*     alter system set undo_tablespace=undotbs2 scope=both;
*   7.等待原UNDO表空间所有UNDO SEGMENT OFFLINE
*     select usn,xacts,status,rssize/1024/1024/1024,hwmsize/1024/1024/1024,shrinks from v$rollstat order by rssize;
*   8.再执行看UNDO表空间所有UNDO SEGMENT ONLINE(和7一样，结果不同？)
*     select usn,xacts,status,rssize/1024/1024/1024,hwmsize/1024/1024/1024,shrinks from v$rollstat order by rssize;
*   9.删除原有的UNDO表空间
*     drop tablespace undotbs1 including contents;
*   10.确认删除是否成功
*     select name from v$tablespace;
*   11.重启数据库或者重启计算机后到存储数据文件的路径下删除数据文件(上面的操作只删除了undo表空间的逻辑关系，未删除文件)
*******************************************************************************************************/

class COracleTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( COracleTester );

    CPPUNIT_TEST_SUITE_END();

public:
    COracleTester();
    ~COracleTester();
    virtual void setUp();
    virtual void tearDown();
private:
};