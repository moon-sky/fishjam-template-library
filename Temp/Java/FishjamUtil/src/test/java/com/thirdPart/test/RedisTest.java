package com.thirdPart.test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import junit.framework.TestCase;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import redis.clients.jedis.Jedis;  
import redis.clients.jedis.JedisPool;  
import redis.clients.jedis.JedisPoolConfig;  

/**********************************************************************************************************************************************
 * blog.csdn.net/freebird_lb/article/details/7733979
 * 
 * TODO: 
 *   1.命令行下 get 出来的字符串，采用unicode编码方式显示，无法显示对应的字符。如何设置？
 *   2.如何更新到磁盘上进行持久化？
**********************************************************************************************************************************************/

//http://blog.csdn.net/renfufei/article/details/38474435
/**********************************************************************************************************************************************
 * Redis(Remote Dictionary Server) -- http://redis.io/download 
 *   开源，提供多种语言(Java，C++，python 等)的API
 *   可基于内存亦可持久化的日志型、Key-Value数据库,和Memcached(只能操作string、且不能持久化?)类似。
 *   支持很多value类型,包括 string、list、set、zset(sorted set)和 hash 等。
 *   支持push/pop、add/remove及取交集并集和差集及更丰富的操作。
 *   为了保证效率，数据都是缓存在内存中。区别的是redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，
 *   Redis同样支持主从复制(master-slave replication)，且具有非常快速的非阻塞首次同步(non-blockingfirst synchronization),
 *     网络断开自动重连等功能。
 *   并且在此基础上实现了master-slave。
 **********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * Linux下源码方式安装并设置
 *   解压后 + make, 然后可执行 redis-server + redis-cli 测试
 * 
 * 1.文件列表及说明
 *   redis-server 		-- 服务器程序
 *   redis-cli			-- 客户端程序
 *   redis-check-dump	-- 本地数据库检查
 *   redis-check-aof	    -- 更新日志检查
 *   redis-benchmark	    -- 性能基准测试,用以模拟同时由N个客户端发送M个 SETs/GETs 查询(类似于Apache的ab工具)
 *   redis.conf		    -- 配置文件
**********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * Windows下安装并设置Redis（http://blog.csdn.net/renfufei/article/details/38474435）
 *   注意：这种方式不是官方支持的，通常只用于加速开发、调试，只有64位版本
 * 0.使用管理员账号，如出现权限问题，可能需要设置属性中的 兼容性权限(以管理员身份运行)
 *   下载: https://github.com/MSOpenTech/redis/releases/download/win-2.8.17.3/redis-2.8.17.zip
 * 1.修改 conf 配置文件 -- 如果不修改会提示 "--maxheap" 的错误
 *   将 "# maxheap <bytes>" 改为 "maxheap 512000000"  -- 即指定为 512M
 * 2.启动，配置文件中的默认端口为 6379
 *   redis-server  redis.windows.conf
 * 3.redis-cli <== 默认连接本地? 可通过 -h <host> -p <port> 的方式指定
 *   然后可通过 set/get 等命令进行测试。
 *   如 set author fishjam <== 设置 author 变量的值
 *      get author <== 获取 author变量的值，应该显示 "fishjam"
 *   可通过 help [tab] 的方式获取相关命令的帮助
 * 4.停止服务
 *   redis-cli -p 6379 shutdown <== 其中6379是redis的端口号
 * 4.辅助批处理脚本
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
 * JedisPool -- 支持对象池，通过 getResource 获取,通过 returnResource 释放
 **********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * key -- 字符串，不要包含空格、换行。格式约定 "object-type:id:field", 如 "user:1000:password" 等
 * value
 *   string -- 最基本的类型,是二进制安全的,可以包含任何数据(如jpg图片或序列化的对象等), 上限为1G
 *   list -- 每个子元素都是string类型的双向链表, 可通过 push/pup 从头部/尾部 添加删除元素。其操作分为 阻塞版本(生产者消费者队列)和非阻塞版本。
 *   set -- string类型的无序集合，通过hash表方式实现，除了常用的增、删、查等操作外，还有计算集合的 并集、交集、差集(可很容易的实现 sns 中的好友推荐等功能)
 *   sorted set -- 其中的每个元素都会关联一个double类型的score，按score排序，其内部实现是skip list(跳表)和hash table的混合体
 *   hash -- string类型的field 和 value的映射表，特别适合用于存储对象(节约内存、且方便存取整个对象)
 *   
 * 命令(一般来说返回1表示成功) -- 可通过 help xxx 来查询详细的说明
 * http://redis.io/commands， 中文版本: http://redis.readthedocs.org/en/2.4/index.html
 *   Append key value <== 给指定key的字符串值追加value,返回新字符串值的长度
 *   ★DBSize <== 返回当前数据库的key数量
 *   Decr key <== 对key的值做 减一 操作,并返回新的值。decr一个不存在key，则设置key为-1
 *   DecrBy key decrement <== 对key的值做 减指定值 操作
 *   Del key [key ...] <== 删除给定key，返回删除key的数目，0表示给定key都不存在
 *   Exists key <== 测试指定key是否存在，返回1表示存在，0不存在
 *   Expire key seconds <== 为key指定过期时间，单位是秒
 *   FlushAll(慎用) <== 删除所有数据库中的所有key
 *   FlushDB(慎用) <== 删除当前数据库中所有key
 *   Get key <== 获取key对应的string值,如果key不存在返回nil
 *   GetRange key start end <== 返回截取过的key的字符串值,下标从0开始。TODO: 旧版本中是 substr ?
 *   GetSet key value <== 原子的设置key的值，并返回key的旧值。如果key不存在返回nil
 *   INCR key <== 对key的值做 加一 操作,并返回新的值。若value不是int类型会返回错误。incr一个不存在的key，则设置key为1
 *   INCRBY key increment <== 对key的值做 加指定值 的操作
 *   ★KEYS pattern <== 返回匹配指定模式(正则?)的所有key, 如 KEYS k?? 或 KEYS *
 *   MGET key [key ...] <== 一次获取多个key的值，如果对应key不存在，则对应返回nil
 *   MSET key value [key value ...] <== 一次设置多个key的值，成功返回1表示所有的值都设置了，失败返回0表示没有任何值被设置(TODO: 部分成功呢？)
 *   MSETNX key value [key value ...] <== 一次设置多个key的值, 不覆盖
 *   RANDOMKEY <== 返回从当前数据库中随机选择的一个key(TODO:有什么用?)，如果当前数据库是空的，返回空串
 *   MOVE key db <== 将key从当前数据库移动到指定数据库
 *   RENAME key newkey <== 原子的重命名一个key，如果newkey存在，将会被覆盖
 *   RENAMENX key newkey <== 原子的重命名一个key，如果newkey存在返回失败
 *   SELECT index <== 将key从当前数据库移动到指定数据库(TODO:Change the selected database for the current connection)
 *   SET key value <== 设置key对应的值为string类型的value
 *   SETNX key value <== 设置key对应的值为string类型的value, 如果key已经存在，返回0。nx 表示 not exist
 *   TTL key <== 返回设置过过期时间的key的剩余过期秒数, -1表示key不存在或者没有设置过过期时间
 *   TYPE key <== 返回给定key的value类型。如 none(不存在key)，string, list, set 等
 *   
 * Hash 相关
 *   HGET key field <== 获取指定的hash field
 *   HIncrBy key field increment <== 将指定的hash filed 加上给定值
 *   HMGET key field [field ...] <== 获取全部指定的hash filed
 *   HMSET key field value [field value ...] <== 同时设置hash的多个field
 *   HSET key field value <== 设置hash field为指定值，如果key不存在，则先创建
 *
 *   
 * List 相关
 *   B[LR]POP key [key ...] timeout <== 依次扫描key对应的list,找到第一个非空list进行阻塞式的POP操作. timeout单位为秒，0表示一直阻塞
 *   [LR]PUSH key value [value ...] <== 在key对应list的 [头部|尾部] 添加字符串元素，返回值为添加新元素后list的元素个数
 *   [LR]POP key <== 从list的 [头部|尾部] 删除元素，并返回删除元素
 *   ★LLEN key <== 返回key对应list的长度，key不存在返回0
 *   LRANGE key start stop <== 返回指定区间内的元素，下标从0开始，负值表示从后面计算, -1表示倒数第一个元素
 *   LREM key count value <== 从key对应list中删除count个和value相同的元素, count为0时候删除全部
 *   LSET key index value <== 设置list中指定下标的元素值
 *   LTRIM key start stop <== 截取list，保留指定区间内元素
 *   [B]RPOPLPUSH srckey destkey <== 从srckey对应list的尾部移除元素并添加到destkey对应list的头部,最后返回被移除的元素值。带B的是阻塞版本
 *   
 * Set 相关
 *   SADD key member [member ...] <== 添加一个string元素到key对应的set集合中
 *   ★SCARD key <== 返回set的元素个数
 *   SDIFF key [key ...] <== 返回所有给定key的差集
 *   SDIFFSTORE dstkey key [key ...] <== 所有给定key的差集保存到dstkey下
 *   SISMEMBER key member <== 判断member是否在set中，存在返回1，0表示不存在或者key不存在
 *   SINTER key [key ...] <== 返回所有给定key的交集
 *   SINTERSTORE dstkey key [key ...] <== 将所有给定key的交集存到dstkey下
 *   SMEMBERS key <== 返回key对应set的所有元素，结果是无序的
 *   SMOVE srckey dstkey member <== 从srckey对应set中移除member并添加到dstkey对应set中，整个操作是原子的
 *   SPOP key <== 删除并返回key对应set中随机的一个元素
 *   SRANDMEMBER key [count] <== 随机获取set中的count个元素，但是不删除元素
 *   SREM key member [member ...] <== 从key对应set中移除给定元素
 *   SUNION key [key ...] <== 返回所有给定key的并集
 *   SUNIONSTORE dstkey key [key ...] <== 所有给定key的并集保存到dstkey下
 *   
 * Sorted Set 相关
 *   ZADD key score member [score member ...] <== 添加元素到集合，元素在集合中存在则更新对应score
 *   ★ZCARD key <== 返回集合中元素个数
 *   ZCOUNT key min max <== 返回集合中score在给定区间的数量
 *   ZINCRBY key increment member <== 增加对应member的score值，然后移动元素并保持skip list保持有序。返回更新后的score值
 *   ZRANGE|ZREVRANGE  key start stop [WITHSCORES] <== 返回集合中指定区间的元素。返回的是按score[从小到大|从大到小]的有序结果
 *   ZRANGEBYSCORE key min max <== 返回集合中score在给定区间的元素,可指定个数限制
 *   ★ZRANK|ZREVRANK key member <== 返回指定元素在集合中的排名(下标),集合中元素是按score [从小到大|从大到小] 排序的
 *   ZREM key member [member ...] <== 删除指定元素
 *   ZREMRANGEBYRANK key start stop <== 删除集合中排名在给定区间的元素
 *   ZREMRANGEBYSCORE key min max <== 删除集合中score在给定区间的元素
 *   ZSCORE key member <== 返回给定元素对应的score
**********************************************************************************************************************************************/

public class RedisTest extends TestCase {
	JedisPoolConfig	config;
	JedisPool pool;  
    Jedis jedis;  
    
    @Before  
    public void setUp() {
    	//jedis = new Jedis("127.0.0.1", 6379);
    	config = new JedisPoolConfig();
    	config.setMaxIdle(1000);
    	config.setMaxWaitMillis(100000);
    	
        pool = new JedisPool(config, "127.0.0.1", 6379);  
        jedis = pool.getResource();  
        //jedis.auth("password");  
    } 

    @After
	protected void tearDown() throws Exception {
    	if(jedis != null){
    		pool.returnResource(jedis);	//释放回池
        	//jedis.disconnect();    	//直接关闭连接
        	jedis = null;
    	}
    	if(pool != null){
    		pool.destroy();
    		pool = null;
    	}

    	super.tearDown();
	}



	@Test  
    public void testBasicString(){  
        //-----添加数据----------  
        jedis.set("name","fujie");//向key-->name中放入了value-->minxr
        assertEquals("fujie", jedis.get("name"));
  
        //-----修改数据-----------  
        //1、在原来基础上修改  
        jedis.append("name"," fishjam");
        assertEquals("fujie fishjam", jedis.get("name"));
  
        //2、直接覆盖原来的数据  
        jedis.set("name","fishjam");  
        assertEquals("fishjam", jedis.get("name"));
  
        //删除key对应的记录  
        jedis.del("name");  
        assertNull(jedis.get("name"));
  
        /** 
         * mset相当于 
         * jedis.set("name","fishjam"); 
         * jedis.set("birthday", 1979); 
         */  
        jedis.mset("name","fishjam", "birthday","1979");
        List<String> resultList = new ArrayList<String>();
        resultList.add("fishjam");
        resultList.add("1979");
        assertEquals( resultList, jedis.mget("name","birthday"));  
        //System.out.println(jedis.mget("name","birthday"));
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
