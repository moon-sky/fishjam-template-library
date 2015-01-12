package com.thirdPart.test;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import redis.clients.jedis.Jedis;  
import redis.clients.jedis.JedisPool;  
import redis.clients.jedis.JedisPoolConfig;  

/**********************************************************************************************************************************************
 * Redis -- http://redis.io/download , 开源，提供多种语言(Java，C++，python 等)的API
 *   可基于内存亦可持久化的日志型、Key-Value数据库,和Memcached类似。
 *   支持很多value类型,包括 string、list、set、zset(sorted set)和hash。
 *   支持push/pop、add/remove及取交集并集和差集及更丰富的操作。
 *   为了保证效率，数据都是缓存在内存中。区别的是redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，
 *   并且在此基础上实现了master-slave。
 **********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * Windows下安装并设置Redis（http://blog.csdn.net/renfufei/article/details/38474435）
 *   注意：这种方式不是官方支持的，通常只用于加速开发、调试，只有64位版本
 * 0.使用管理员账号，如出现权限问题，可能需要设置属性中的 兼容性权限(以管理员身份运行)
 *   下载: https://github.com/MSOpenTech/redis/releases/download/win-2.8.17.3/redis-2.8.17.zip
 * 1.文件列表及说明
 *   redis-server.exe 		-- 服务器程序
 *   redis-cli.exe			-- 客户端程序
 *   redis-check-dump.exe	-- 本地数据库检查
 *   redis-check-aof.exe	-- 更新日志检查
 *   redis-benchmark.exe	-- 性能基准测试,用以模拟同时由N个客户端发送M个 SETs/GETs 查询(类似于Apache的ab工具)
 *   redis.windows.conf		-- 配置文件
 * 2.修改 conf 配置文件 -- 如果不修改会提示 "--maxheap" 的错误
 *   将 "# maxheap <bytes>" 改为 "maxheap 512000000"  -- 即指定为 512M
 * 3.启动，配置文件中的默认端口为 6379
 *   redis-server  redis.windows.conf
 * 4.redis-cli.exe <== 默认连接本地? 可通过 -h <host> -p <port> 的方式指定
 *   然后可通过 set/get 等命令进行测试。
 *   如 set author fishjam <== 设置 author 变量的值
 *      get author <== 获取 author变量的值，应该显示 "fishjam"
 *   可通过 help [tab] 的方式获取相关命令的帮助
 * 5.辅助批处理脚本
 *   service-install.bat   <== redis-server.exe --service-install redis.windows.conf --loglevel verbose
 *   uninstall-service.bat <== redis-server --service-uninstall
 *   startup.bat           <== redis-server.exe redis.windows.conf
 **********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * Java开发 -- Jedis <== http://flychao88.iteye.com/blog/1527163
 *   JedisPool pool = new JedisPool(new JedisPoolConfig(), "192.168.10.61");  
 *   Jedis jedis = pool.getResource(); 
 *   jedis.auth("password");
 *     
 **********************************************************************************************************************************************/

public class RedisTest {
	JedisPool pool;  
    Jedis jedis;  
    
    @Before  
    public void setUp() {  
        pool = new JedisPool(new JedisPoolConfig(), "127.0.0.1");  
  
        jedis = pool.getResource();  
        jedis.auth("password");  
    } 
    /** 
     * Redis存储初级的字符串 
     * CRUD 
     */  
    @Test  
    public void testBasicString(){  
        //-----添加数据----------  
        jedis.set("name","minxr");//向key-->name中放入了value-->minxr  
        System.out.println(jedis.get("name"));//执行结果：minxr  
  
        //-----修改数据-----------  
        //1、在原来基础上修改  
        jedis.append("name","jarorwar");   //很直观，类似map 将jarorwar append到已经有的value之后  
        System.out.println(jedis.get("name"));//执行结果:minxrjarorwar  
  
        //2、直接覆盖原来的数据  
        jedis.set("name","闵晓荣");  
        System.out.println(jedis.get("name"));//执行结果：闵晓荣  
  
        //删除key对应的记录  
        jedis.del("name");  
        System.out.println(jedis.get("name"));//执行结果：null  
  
        /** 
         * mset相当于 
         * jedis.set("name","minxr"); 
         * jedis.set("jarorwar","闵晓荣"); 
         */  
        jedis.mset("name","minxr","jarorwar","闵晓荣");  
        System.out.println(jedis.mget("name","jarorwar"));  
  
    }  
  
    /** 
     * jedis操作Map 
     */  
    @Test  
    public void testMap(){  
        Map<String,String> user=new HashMap<String,String>();  
        user.put("name","minxr");  
        user.put("pwd","password");  
        jedis.hmset("user",user);  
        //取出user中的name，执行结果:[minxr]-->注意结果是一个泛型的List  
        //第一个参数是存入redis中map对象的key，后面跟的是放入map中的对象的key，后面的key可以跟多个，是可变参数  
        List<String> rsmap = jedis.hmget("user", "name");  
        System.out.println(rsmap);  
  
        //删除map中的某个键值  
//        jedis.hdel("user","pwd");  
        System.out.println(jedis.hmget("user", "pwd")); //因为删除了，所以返回的是null  
        System.out.println(jedis.hlen("user")); //返回key为user的键中存放的值的个数1  
        System.out.println(jedis.exists("user"));//是否存在key为user的记录 返回true  
        System.out.println(jedis.hkeys("user"));//返回map对象中的所有key  [pwd, name]  
        System.out.println(jedis.hvals("user"));//返回map对象中的所有value  [minxr, password]  
  
        Iterator<String> iter=jedis.hkeys("user").iterator();  
        while (iter.hasNext()){  
            String key = iter.next();  
            System.out.println(key+":"+jedis.hmget("user",key));  
        }  
  
    }  
  
    /** 
     * jedis操作List 
     */  
    @Test  
    public void testList(){  
        //开始前，先移除所有的内容  
        jedis.del("java framework");  
        System.out.println(jedis.lrange("java framework",0,-1));  
       //先向key java framework中存放三条数据  
       jedis.lpush("java framework","spring");  
       jedis.lpush("java framework","struts");  
       jedis.lpush("java framework","hibernate");  
       //再取出所有数据jedis.lrange是按范围取出，  
       // 第一个是key，第二个是起始位置，第三个是结束位置，jedis.llen获取长度 -1表示取得所有  
       System.out.println(jedis.lrange("java framework",0,-1));  
    }  
  
    /** 
     * jedis操作Set 
     */  
    @Test  
    public void testSet(){  
        //添加  
        jedis.sadd("sname","minxr");  
        jedis.sadd("sname","jarorwar");  
        jedis.sadd("sname","闵晓荣");  
        jedis.sadd("sanme","noname");  
        //移除noname  
        jedis.srem("sname","noname");  
        System.out.println(jedis.smembers("sname"));//获取所有加入的value  
        System.out.println(jedis.sismember("sname", "minxr"));//判断 minxr 是否是sname集合的元素  
        System.out.println(jedis.srandmember("sname"));  
        System.out.println(jedis.scard("sname"));//返回集合的元素个数  
    }  
  
    @Test  
    public void test() throws InterruptedException {  
        //keys中传入的可以用通配符  
        System.out.println(jedis.keys("*")); //返回当前库中所有的key  [sose, sanme, name, jarorwar, foo, sname, java framework, user, braand]  
        System.out.println(jedis.keys("*name"));//返回的sname   [sname, name]  
        System.out.println(jedis.del("sanmdde"));//删除key为sanmdde的对象  删除成功返回1 删除失败（或者不存在）返回 0  
        System.out.println(jedis.ttl("sname"));//返回给定key的有效时间，如果是-1则表示永远有效  
        jedis.setex("timekey", 10, "min");//通过此方法，可以指定key的存活（有效时间） 时间为秒  
        Thread.sleep(5000);//睡眠5秒后，剩余时间将为<=5  
        System.out.println(jedis.ttl("timekey"));   //输出结果为5  
        jedis.setex("timekey", 1, "min");        //设为1后，下面再看剩余时间就是1了  
        System.out.println(jedis.ttl("timekey"));  //输出结果为1  
        System.out.println(jedis.exists("key"));//检查key是否存在  
        System.out.println(jedis.rename("timekey","time"));  
        System.out.println(jedis.get("timekey"));//因为移除，返回为null  
        System.out.println(jedis.get("time")); //因为将timekey 重命名为time 所以可以取得值 min  
  
        //jedis 排序  
        //注意，此处的rpush和lpush是List的操作。是一个双向链表（但从表现来看的）  
        jedis.del("a");//先清除数据，再加入数据进行测试  
        jedis.rpush("a", "1");  
        jedis.lpush("a","6");  
        jedis.lpush("a","3");  
        jedis.lpush("a","9");  
        System.out.println(jedis.lrange("a",0,-1));// [9, 3, 6, 1]  
        System.out.println(jedis.sort("a")); //[1, 3, 6, 9]  //输入排序后结果  
        System.out.println(jedis.lrange("a",0,-1));  
  
    }  
}
