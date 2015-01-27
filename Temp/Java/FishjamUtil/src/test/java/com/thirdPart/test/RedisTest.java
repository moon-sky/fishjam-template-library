package com.thirdPart.test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.servlet.jsp.tagext.TryCatchFinally;

import junit.framework.TestCase;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import redis.clients.jedis.Jedis;  
import redis.clients.jedis.JedisPool;  
import redis.clients.jedis.JedisPoolConfig;  
import redis.clients.jedis.JedisPubSub;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.Pipeline;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;
import redis.clients.jedis.SortingParams;
import redis.clients.jedis.Transaction;
import redis.clients.util.Hashing;
import redis.clients.util.Sharded;

/**********************************************************************************************************************************************
 * blog.csdn.net/freebird_lb/article/details/7733979
 * 
 * TODO:
 *   0.redis 是单线程的 ? Windows 版本时通过 info 查询其实现方式为 IOCP，multiplexing_api:winsock_IOCP
 *   1.命令行下 get 出来的字符串，采用unicode编码方式显示，无法显示对应的字符。如何设置？
 *   2.如何更新到磁盘上进行持久化？
 *   
 * 注意:
 *   1.如果要sort的集合非常大的话排序就会消耗很长时间, 会阻塞其他client的请求
 *     解决方案: a.通过主从复制机制将数据复制到多个slave上。然后我们只在slave上做排序操作。并进可能的对排序结果缓存。
 *                   b.采用sortedset对需要按某个顺序访问的集合建立索引
**********************************************************************************************************************************************/


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
 *     info <== 查看状态信息(包括版本、保存的数据、内存状态等)
 *   redis-check-dump	-- 本地数据库检查
 *   redis-check-aof -- 更新日志检查，通常用于检查并修复失败的事务等，否则 redis 重启时检测到失败时不能正常启动。
 *   redis-benchmark -- 性能基准测试,用以模拟同时由N个客户端发送M个 SETs/GETs 查询(类似于Apache的ab工具)
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
 * Pipeline -- 从client打包多条命令一起发出，不需要等待单条命令的响应返回。服务端处理完多条命令后会将多条命令的处理结果打包返回给客户端，从而提高性能。
 *                 注意：服务器端必须在处理完所有命令前先缓存起所有命令的处理结果，占用较多的内存。
 * JedisPubSub -- 发布订阅的抽象基类
 *  ShardedJedisPool  -- TODO: 用于集群的?
 **********************************************************************************************************************************************/

/**********************************************************************************************************************************************
* 事务支持
*   注意:
*      1.目前版本(新版本?)只能保证一个client发起的事务中的命令可以连续的执行，而中间不会插入其他client的命令
*      2.只能保证事务的每个命令连续执行，但是如果事务中的一个命令失败了，并不回滚其他命令
*   
*   Multi [进入事务] => 将命令放入队列进行缓冲 => Exec(立即顺序执行所有命令)
*   Discard : 清空事务的命令队列并退出事务上下文
*   Watch key : 在事务前监视给定的key, 当exec时候如果监视的key从调用watch后发生过变化，则整个事务会失败 。通常用于实现乐观锁
*   UnWatch key: 
*   
* PubSub(发布订阅) -- 解耦消息发布者和消息订阅者之间耦合的一种消息通信模式(类似观察者模式)
*    Subscribe  | UnSubscribe  channel [channel...] :  订阅者向redis server订阅自己感兴趣的消息 channel
*    PSubscribe| PUnSubscribe pattern [pattern ...] : 订阅者向redis server订阅自己感兴趣的消息 Pattern(可使用通配符等指定多个，如 "news.*" )
*    Publish channel message :  发布者向redis server发送特定类型的消息(此后订阅该消息的全部client会收到此消息, 消息传递为多对多)
*    注意:
*      redis的pub/sub还是有点太单薄（实现才用150行代码）。在安全，认证，可靠性这方便都没有太多支持
*      
* 持久化 -- 支持四种持久化方式(通过 redis.conf 配置)
*    Snapshotting(快照) -- 默认方式. 将内存中数据以快照的方式写入到二进制文件中(默认为 dump.rdb )。如 "save 300 10" 表示 在10秒内如果超过300个key被修改就自动快照保存。
*       问题: 每次快照持久化都是将内存数据完整写入到磁盘一次(而非增量同步脏数据)，当数据量大且变更频繁时，可能会严重影响性能。
*    Append-only file(aof) -- 将每一个收到的写命令都通过write函数追加到文件中(appendonly.aof), 重启时通过重新执行其中保存的写命令来在内存中重建整个数据库内容。
*       需要将 "appendonly " 配置项设为 "yes" 来启用， 通过 appendfsync 配置调用 fsync 的时机。
*       问题: 持久化文件会变的越来越大， 可通过 bgrewriteaof 命令重写aof文件( 将当前内存中的数据以命令方式写入并替换原来的持久化文件 )
*    Virtual Memory(虚拟内存, 因为慢而且复杂, 已被废弃) -- 通过 "vm-enabled" 配置项启用
*    diskstore -- 原理很美好，但目前还不完善，有待发展和测试确认
*      读操作: 使用 read through 以及LRU方式 -- 内存中不存在的数据从磁盘拉取并放入内存，内存中放不下的数据采用LRU淘汰
*      写操作: 另外一个子线程单独处理，写线程通常是异步的, 通过 "cache-flush-delay" 配置写延迟
*    
**********************************************************************************************************************************************/

/**********************************************************************************************************************************************
* 主从复制 -- 允许多个slave server拥有和master server相同的数据库副本
*   配置项:
*     slaveof <masterip> <masterport> --  配置slave服务器,指定 mater 的ip和端口
* 
* 分布式(集群) -- 作者说将在 3.0 中实现集群机制
*    目前的方式(参见 testShardedJedis ): 采用一致性哈稀分片(Shard)，将不同的key分配到不同的redis server上, 达到横向扩展的目的
*        问题: 1. 扩容时增加机器后，同样的key算出来会落到与原来不同的机器上，造成取不到原有的值(通过 Pre-Sharding 方式解决?)
*                2.单点故障问题 -- 运用主从复制解决?
**********************************************************************************************************************************************/

/**********************************************************************************************************************************************
 * key -- 字符串(区分大小写)，不要包含空格、换行。格式约定 "object-type:id:field", 如 "user:1000:password" 等
 * value
 *   string -- 最基本的类型,是二进制安全的,可以包含任何数据(如jpg图片或序列化的对象等), 上限为1G
 *   list -- 每个子元素都是string类型的双向链表, 可通过 push/pup 从头部/尾部 添加删除元素。其操作分为 阻塞版本(生产者消费者队列)和非阻塞版本。
 *            内部实现: 元素个数小于 list-max-ziplist-entries[512] 且 元素值字符串的长度小于 list-max-ziplist-value[64]， 则使用 ziplist 存储; 否则 使用 Dict 存储
 *   set -- string类型的无序集合，除了常用的增、删、查等操作外，还有计算集合的 并集、交集、差集(可很容易的实现 sns 中的好友推荐等功能)
 *            内部实现: 元素为整数且元素个数小于 set-max-intset-entries[512] 时使用 intset 存储，否则使用 Dict 存储
 *   sorted set -- 其中的每个元素都会关联一个double类型的score，按score排序，其内部实现是skip list(跳表)和hash table的混合体
 *            内部实现: 元素个数小于 zset-max-ziplist-entries[128] 且 元素值字符串的长度小于 zset-max-ziplist-value[64] 时, 使用 ziplist 存储. 否则?
 *   hash -- 在一个 key 下可以保存多个string类型的 field 和 value, 特别适合用于存储对象(节约内存、且方便存取整个对象),
 *              内部实现: 初识时使用 zipmap 来存储(效率稍低但节约内存). 如果field 数超过限制( hash-max-zipmap-entries[512], hash-max-zipmap-value[64] )，则自动转换为hash. 
 *              
 *   
 * 命令(一般来说返回1表示成功) -- 可通过 help xxx 来查询详细的说明
 * http://redis.io/commands， 中文版本: http://redis.readthedocs.org/en/2.4/index.html
 *   
 * Key相关
 *   ★DBSize <== 返回当前数据库的key数量
 *   Del key [key ...] <== 删除给定key，返回删除key的数目，0表示给定key都不存在
 *   Exists key <== 测试指定key是否存在，返回1表示存在，0不存在
 *   Expire key seconds <== 为key指定过期时间，单位是秒
 *   FlushAll(慎用) <== 删除所有数据库中的所有key
 *   FlushDB(慎用) <== 删除当前数据库中所有key
 *   ★KEYS pattern <== 返回匹配指定模式(正则?)的所有key, 如 KEYS k?? 或 KEYS *
 *   MOVE key db <== 将key从当前数据库移动到指定数据库
 *   RANDOMKEY <== 返回从当前数据库中随机选择的一个key(TODO:有什么用?)，如果当前数据库是空的，返回空串
 *   RENAME key newkey <== 原子的重命名一个key，如果newkey存在，将会被覆盖
 *   RENAMENX key newkey <== 原子的重命名一个key，如果newkey存在返回失败
 *   SELECT index <== 将key从当前数据库移动到指定数据库(TODO:Change the selected database for the current connection)
 *   TTL key <== 返回设置过过期时间的key的剩余过期秒数, -1表示key不存在或者没有设置过过期时间
 *   TYPE key <== 返回给定key的value类型。如 none(不存在key)，string, list, set 等
 *   
 * 通用操作
 *   Sort key [BY pattern] [LIMIT offset count] [ASC|DESC] [ALPHA] [STORE dstkey] ... <== 对 list，se t和 sorted set 元素的排序( 不支持 hash )
 *     
 * String相关( [key] => [value] )
 *   Append key value <== 给指定key的字符串值追加value,返回新字符串值的长度
 *   Decr key <== 对key的值做 减一 操作,并返回新的值。decr一个不存在key，则设置key为-1
 *   DecrBy key decrement <== 对key的值做 减指定值 操作
 *   Get key <== 获取key对应的string值,如果key不存在返回nil
 *   GetRange key start end <== 返回截取过的key的字符串值,下标从0开始。TODO: 旧版本中是 substr ?
 *   GetSet key value <== 原子的设置key的值，并返回key的旧值。如果key不存在返回nil
 *   INCR key <== 对key的值做 加一 操作,并返回新的值。若value不是int类型会返回错误。incr一个不存在的key，则设置key为1
 *   INCRBY key increment <== 对key的值做 加指定值 的操作
 *   MGET key [key ...] <== 一次获取多个key的值，如果对应key不存在，则对应返回nil
 *   MSET key value [key value ...] <== 一次设置多个key的值，成功返回1表示所有的值都设置了，失败返回0表示没有任何值被设置(TODO: 部分成功呢？)
 *   MSETNX key value [key value ...] <== 一次设置多个key的值, 不覆盖
 *   SET key value <== 设置key对应的值为string类型的value
 *   SetEx key seconds value <== 设置具有超时的value，超时单位为秒
 *   SETNX key value <== 设置key对应的值为string类型的value, 如果key已经存在，返回0。nx 表示 not exist
 *   
 *   
 * Hash 相关( [key] =>[ { field : value } ... ] )
 *   HDel key field <== 删除指定的hash field
 *   HExists key field <== 测试指定field是否存在
 *   HGet key field <== 获取指定的hash field
 *   HGetAll key <== 返回hash的所有filed和value 
 *   HIncrBy key field increment <== 将指定的hash filed 加上给定值
 *   ★HKeys key <== 返回hash的所有field
 *   ★HLen key <== 返回指定hash的field数量
 *   HMGet key field [field ...] <== 获取全部指定的hash filed
 *   HMSet key field value [field value ...] <== 同时设置hash的多个field
 *   HSet key field value <== 设置hash field为指定值，如果key不存在，则先创建
 *   HVals key <== 返回hash的所有value
 *   
 * List 相关 ( [key] => [value ...]  )
 *   B[LR]POP key [key ...] timeout <== 依次扫描key对应的list,找到第一个非空list进行阻塞式的POP操作. timeout单位为秒，0表示一直阻塞
 *   [LR]PUSH key value [value ...] <== 在key对应list的 [头部|尾部] 添加字符串元素，返回值为添加新元素后list的元素个数
 *   [LR]POP key <== 从list的 [头部|尾部] 删除元素，并返回删除元素
 *   ★LLEN key <== 返回key对应list的长度，key不存在返回0
 *   LRANGE key start stop <== 返回指定区间内的元素，下标从0开始，负值表示从后面计算, -1表示倒数第一个元素
 *   LREM key count value <== 从key对应list中删除count个和value相同的元素, count为0时候删除全部
 *   LSET key index value <== 设置list中指定下标的元素值
 *   LTRIM key start stop <== 截取list，保留指定区间内元素, 如果范围时 1~0 的话，表示清除所有元素
 *   [B]RPOPLPUSH srckey destkey <== 从srckey对应list的尾部移除元素并添加到destkey对应list的头部,最后返回被移除的元素值。带B的是阻塞版本
 *   
 * Set 相关( [key]  => [value ...] )
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
	static JedisPoolConfig	config;
	static JedisPool pool;  
    Jedis jedis;  
    
    static 
    {
    	config = new JedisPoolConfig();
    	config.setMaxIdle(1000);
    	config.setMaxWaitMillis(100000);
        pool = new JedisPool(config, "127.0.0.1", 6379);  
        //TODO:释放?
    	/*
        {
        	if(pool != null){
        		pool.destroy();
        		pool = null;
        	}
        }
        */
    }
    
    @Before  
    public void setUp() {
    	//jedis = new Jedis("127.0.0.1", 6379);
        jedis = pool.getResource();  
        //jedis.auth("password");  
        jedis.incr("userId");
    } 

    @After
	protected void tearDown() throws Exception {
    	if(jedis != null){
    		jedis.decr("userId");
    		pool.returnResource(jedis);	//释放回池
        	//jedis.disconnect();    	//直接关闭连接
        	jedis = null;
    	}
    	super.tearDown();
	}



	@Test  
    public void testBasicString(){
		//在 setUp 中 incr("userId")递增, 然后这里可以用其作为 DB 中的 PrimaryKey 进行处理, 实际中应该是通过db进行初始化，使用前 incr ?
		//TODO: 为了保证和数据库的同步，最好用数据库中自动生成的Key?
		//assertEquals("1", jedis.get("userId"));
		
        //-----添加数据----------  
        jedis.set("name","fujie");  //向key-->name中放入了value-->fujie
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
     * jedis操作Map(即Hash) 
     */  
    @Test  
    public void testMap(){  
        Map<String,String> user=new HashMap<String,String>();  
        user.put("name","fishjam");  
        user.put("birthday","1979");  
        jedis.hmset("user",user);  
        
        //取出user中的name，结果是一个泛型的List, 结果按照key排序
        //第一个参数是存入redis中map对象的key，后面跟的是放入map中的对象的key，后面的key可以跟多个，是可变参数  
        List<String> resultMap = new ArrayList<String>();
        resultMap.add("1979");
        resultMap.add("fishjam");
        resultMap.add(null);
        assertEquals(resultMap, jedis.hmget("user", "birthday", "name", "notexist"));

        Set<String> setKeys = new HashSet<String>();
        setKeys.add("birthday");
        setKeys.add("name");
        assertEquals(setKeys, jedis.hkeys("user"));	//返回map对象中的所有key
        
        List<String> listVals = new ArrayList<String>();
        listVals.add("fishjam");
        listVals.add("1979");
        assertEquals(listVals, jedis.hvals("user"));  //返回map对象中的所有value  

        //删除map中的某个键值  
        jedis.hdel("user","birthday"); 
        resultMap.clear();
        resultMap.add(null);
        assertEquals(resultMap, jedis.hmget("user", "birthday")); //因为删除了，所以返回的是空List  
        assertEquals(new Long(1), jedis.hlen("user"));   //返回key为user的键中存放的值的个数1  
        assertTrue(jedis.exists("user"));//是否存在key为user的记录 返回true  
    }  
  
    /** 
     * jedis操作List 
     */  
    @Test  
    public void testList(){
    	jedis.ltrim("framework", 1, 0);		//清除所有数据
    	assertEquals(new Long(0), jedis.llen("framework"));

        //先向key java framework中存放三条数据 , rpush 表示从后面加，如果是 lpush 表示从前面加 
    	jedis.rpush("framework","spring");  
        jedis.rpush("framework","struts");  
        jedis.rpush("framework","hibernate");  
    	
        List<String> resultList = new ArrayList<String>();
        resultList.add("spring");
        resultList.add("struts");
        resultList.add("hibernate");
        //再取出所有数据jedis.lrange是按范围取出，0 -> -1 表示全部
        // 第一个是key，第二个是起始位置，第三个是结束位置，jedis.llen获取长度 -1表示取得所有  
        assertEquals(resultList, jedis.lrange("framework", 0, -1));
    }  
  
    /** 
     * jedis操作Set 
     */  
    @Test  
    public void testSet(){  
        //添加  
        jedis.sadd("season1","spring");  
        jedis.sadd("season1","summer");  
        jedis.sadd("season1","autumn");  
        assertEquals(new Long(3),  jedis.scard("season1"));
        
        jedis.sadd("season2", "winter");
        jedis.sadd("season2", "spring");
        assertEquals(new Long(2),  jedis.scard("season2"));

        assertEquals("[winter, autumn, summer, spring]", jedis.sunion("season1", "season2").toString() );		//两个set的并集
        assertEquals("[spring]", jedis.sinter("season1", "season2").toString() );		//两个set的交集
        
        //两个set的差集 -- 顺序不一样，结果不一样
        assertEquals("[summer, autumn]", jedis.sdiff("season1", "season2").toString());	//属于 1, 不属于 2
        assertEquals("[winter]", jedis.sdiff("season2", "season1").toString());					//属于 2, 不属于 1
        
        jedis.sunionstore("seasonAll", "season1", "season2");
        
        jedis.sadd("season","noname");  
        //移除noname  
        jedis.srem("sname","noname");  
        assertEquals(4, jedis.smembers("seasonAll").size() );	//获取所有加入的value  
        assertTrue(jedis.sismember("seasonAll", "winter"));	//判断 winter 是否是seasonAll集合的元素  
    }  
  
    @Test  
    public void testExpiration() throws InterruptedException {
    	jedis.set("normalKey", "ttlTime");
        assertEquals(new Long(-1), jedis.ttl("normalKey"));		//返回给定key的有效时间，如果是-1则表示永远有效
        
        jedis.setex("timekey", 10, "ttlTime");//通过此方法，可以指定key的存活（有效时间） 时间为秒
        assertEquals(new Long(10), jedis.ttl("timekey"));		//返回给定key的有效时间，如果是-1则表示永远有效
        Thread.sleep(1000);//睡眠2秒后，剩余时间将为 <=8
        //System.out.println("timeKey=" + jedis.ttl("timekey"));
        assertTrue(jedis.ttl("timekey") <= 9 );
        
        jedis.setex("timekey", 1, "ttlTime");        //设为1后，下面再看剩余时间就是1了 
        assertEquals("ttlTime", jedis.get("timekey"));
        Thread.sleep(1500);
        assertFalse(jedis.exists("timekey"));
        assertNull(jedis.get("timekey"));		//超时了，会自动被移除，返回null
    }
    
    @Test
    public void testSort(){
        //jedis 排序  
        //注意，此处的rpush和lpush是List的操作。是一个双向链表（但从表现来看的）  
        jedis.del("testSort");	//先清除数据，再加入数据进行测试  
        jedis.lpush("testSort", "1");  
        jedis.lpush("testSort","6");  
        jedis.lpush("testSort","3");  
        jedis.lpush("testSort","9"); 

        assertEquals("[9, 3, 6, 1]", jedis.lrange("testSort", 0, -1).toString());	//原始插入顺序
        //默认排序 -- Asc(从小到大), 
        assertEquals("[1, 3, 6, 9]", jedis.sort("testSort").toString());		//排序后结果
        assertEquals("[9, 3, 6, 1]", jedis.lrange("testSort",0,-1).toString());	//原始数据不变
        
        //通过 SortingParams 控制排序依据和结果 -- 示例中是 倒序, 只取 [0~2] 共三个结果元素
        SortingParams sortingParams = new SortingParams();
        assertEquals("[9, 6, 3]", jedis.sort("testSort", sortingParams.desc().limit(0, 3)).toString());
        
        jedis.sadd("testSortAlpha", "baidu", "naver", "google");
        assertEquals("[naver, google, baidu]", jedis.sort("testSortAlpha", sortingParams.alpha().desc() ).toString());
        
        //按给定 pattern 组合成新的key，并按照新key中对应的内容进行排序 -- 相当于先过滤? pattern 支持 正则吗?
        //TODO: Get操作可以将list中的值与pattern组成新的key，用该新key去获取出来的list长度个数的值进行排序(排序是排列的get出的值，而不是list中的值)，如果用新组成的key不存在，则返回(nil)
        //具体用法需要查询资料并进一步测试
        //特殊符号:
        //  "#"  : 表示 引用的是原始集合
        //  "->" : 表示引用hash类型字段
        //assertEquals("[baidu, naver]", jedis.sort("testSortAlpha", sortingParams.by("?????").alpha() ).toString());  //以为是选出只有5个字母的值排序，实测不是
    }
    
    @Test
    public void testTransaction()
    {
    	final String STR_TEST_TRANSACTION = "testTransaction";
    	jedis.del(STR_TEST_TRANSACTION);
    	assertNull(jedis.get(STR_TEST_TRANSACTION));
    	
    	Transaction transaction = jedis.multi();
    	transaction.incr(STR_TEST_TRANSACTION);
    	
    	//取消事务
    	transaction.discard();
    	assertNull(jedis.get(STR_TEST_TRANSACTION));
    	
    	transaction = jedis.multi();
    	transaction.incr(STR_TEST_TRANSACTION);
    	transaction.incr(STR_TEST_TRANSACTION);
    	transaction.exec();
    	
    	assertEquals("2", jedis.get(STR_TEST_TRANSACTION));
    	jedis.del(STR_TEST_TRANSACTION);
    }
    
    @Test
    public void testPipeline(){
    	//利用 Pipeline 打包发送命令，提高性能
    	final String STR_TEST_PIPELINE = "testPipeline";
    	jedis.del(STR_TEST_PIPELINE);
    	Pipeline pipeline = jedis.pipelined();
    	for (int i = 0; i < 100; i++) {
			pipeline.incr(STR_TEST_PIPELINE);
		}
    	pipeline.sync();
    	
    	assertEquals("100" , jedis.get(STR_TEST_PIPELINE));
    }
    
    @Test
    public void testPubSub(){
    	final String STR_TEST_PUB_SUB_CHANNEL = "testPubSubChannel";
    	final String STR_TEST_PUB_SUB_PATTERN = "testPubSubPattern";
    	//订阅者可以通过 subscribe 和 psubscribe 命令向redis server订阅自己感兴趣的消息类型
    	final JedisPubSub pubSubHandler = new JedisPubSub() {
			
			@Override
			public void onUnsubscribe(String channel, int subscribedChannels) {
				System.out.println("onUnsubscribe channel=" +  channel + ", subscribedChannels=" + subscribedChannels);
			}
			
			@Override
			public void onSubscribe(String channel, int subscribedChannels) {
				System.out.println("onSubscribe channel=" +  channel + ", subscribedChannels=" + subscribedChannels);
			}

			@Override
			public void onPSubscribe(String pattern, int subscribedChannels) {
				System.out.println("onPSubscribe pattern=" +  pattern + ", subscribedChannels=" + subscribedChannels);
			}
			
			@Override
			public void onPUnsubscribe(String pattern, int subscribedChannels) {
				System.out.println("onPUnsubscribe pattern=" +  pattern + ", subscribedChannels=" + subscribedChannels);
			}
			
			@Override
			public void onPMessage(String pattern, String channel, String message) {
				System.out.println("onPMessage pattern=" +  pattern + ",channel=" + channel + ", message=" + message);
			}
			
			@Override
			public void onMessage(String channel, String message) {
				System.out.println("onMessage channel=" + channel + ", message=" + message);
			}
		};
		
		//注意: 当调用该语句时，程序会进入订阅模式(在 PubSub 的循环中等待 从服务器接收到的消息, TODO: 怎么退出循环? 其他线程中通过 pubSubHandler.unsubscribe() ?)。
		//   然后可在 resc_cli 客户端程序中，通过 "Publish testPubSubChannel 消息" 或 "Publish testPubSubPattern 消息"
		//   的方式发布指定消息，并查看日志
		
		
		//启动一个线程，等待5秒以后结束
		new Runnable() {
			public void run() {
				try {
					Thread.sleep(5000);
					//if (pubSubHandler.isSubscribed()) 
					{
						pubSubHandler.unsubscribe();
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}.run();
		System.out.println("Before jedis.subscribe");
		//jedis.subscribe(pubSubHandler,  STR_TEST_PUB_SUB_CHANNEL);
		//jedis.psubscribe(pubSubHandler, STR_TEST_PUB_SUB_PATTERN);
		System.out.println("After jedis.subscribe");
    }
    
    @Test
    public void testShardedJedis(){
    	assertEquals(1, 1);
    	
    	//测试集群的代码 -- 
    	JedisPoolConfig config =new JedisPoolConfig();//Jedis池配置
        //config.setMaxActive(500);//最大活动的对象个数
        config.setMaxIdle(1000 * 60);//对象最大空闲时间
        //config.setMaxWait(1000 * 10);//获取对象时最大等待时间
        config.setTestOnBorrow(true);
        
        String hostA = "10.10.224.44";
        int portA = 6379;
        String hostB = "10.10.224.48";
        int portB = 6379;
        
        List<JedisShardInfo> jdsInfoList =new ArrayList<JedisShardInfo>(2);
        JedisShardInfo infoA = new JedisShardInfo(hostA, portA);
        JedisShardInfo infoB = new JedisShardInfo(hostB, portB);
        jdsInfoList.add(infoA);
        jdsInfoList.add(infoB);
        
        ShardedJedisPool pool =new ShardedJedisPool(config, jdsInfoList, Hashing.MURMUR_HASH, Sharded.DEFAULT_KEY_TAG_PATTERN);
        
        ShardedJedis jds = null;
        for (int i = 0; i < 3; i++) {
        	jds = pool.getResource();
        	try {
    			String key = "key:" + i;
    			System.out.println(key+":"+jds.getShard(key).getClient().getHost());  //不同的key被分配到不同的Redis-Server上去
    			System.out.println(jds.set(key,"1111111111111111111111111111111"));
			} finally {
				pool.returnResource(jds);
			}
		}
    }
}

