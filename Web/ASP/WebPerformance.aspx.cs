using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Caching;

/************************************************************************************************************
 * System.Text.StringBuilder -- 处理密集的字符串操作，使用内部缓存来处理字符串
 * 
 * 缓存 -- 通常只缓存开销昂贵并且频繁使用的数据
 *   输出缓存 -- 为发送给客户端的、最终生成的HTML页面缓存一个副本，常用于静态网页，缓存以后通常不再能交互?
 *     Page 或 Control 指令中加入 OutputCache Duration="30" VaryByParma="*" -- 该页面缓存30秒，根据不同的查询字符串参数来缓存多个不同的副本
 *     可以重载 GetVaryByCustomString 方法生成自定义的缓存字符串(不一样的缓存字符串会生成新的缓存)
 *   数据缓存(将数据保存在 Page.Cache 中 ) -- Cache是线程安全的，但其中缓存对象的线程安全性需要自行保证
 *     if(Cache["itemName"] == null) { 没有缓存则获取数据后 Cache.Insert("itemName", xxx); } 返回数据
 *   数据源控件(SqlDataSource, ObjectDataSource 等)的内建缓存：EnableCaching 属性设置为true, CacheExpirationPolicy 设置过期方式
 *   
 *   缓存依赖 -- 允许在缓存项和其他资源之间建立依赖关系，当所依赖的资源发生变化时，缓存项将被自动移除
 *     CacheDependency prodDependency = new CacheDependency(Server.MapPath("ProductList.xml")); //依赖于指定文件
 *     如要启用对数据库表的缓存依赖(小心使用)，需要数据库支持(SQLServer2000),
 *       1.通过 aspnet_regsql.exe -ed 启用其通知功能(DB内部通过触发器将变化写入 SqlCacheTablesForNotification 表)
 *       2.启用 ASP.NET 的轮询服务，查找变化并通知, 15秒轮询一次:web.config 中 <sqlCacheDependency enabled="true" pollTime="15000"> 
 *       3.代码中创建缓存依赖对象 SqlCacheDependency, 并放入 Cache 中
 *       SQLServer2008比较智能，采用服务代理的方式通知，ASP.NET 不再轮询，而是采用监听事件的方式，需要调用 SqlDependency.Start(xxx)初始化Web服务器上的监听服务
 *        
 * 页面跟踪(ASP.NET 3.5从入门到精通 P257~258,缺页) -- 可以跟踪页面的详细信息，可用于性能分析？
 *   控件树、会话状态和应用程序状态。
 *   
 * Web农场(Web Farms) -- 共同负责处理HTTP请求的一组Web服务器，专用于需要处理重负载的高性能网站。即Web 的分布式扩展？
************************************************************************************************************/

public partial class WebPerformance : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
         
    }
}
