using System;

/*******************************************************************************************************************
 * ISV -- 第三方开发者
 * 
 * 配置沙箱环境(利用沙箱环境模拟正式环境中的调用) -- 可实现：沙箱下面APPKEY申请，自用型应用绑定等功能
 *   注意：买家应用和淘宝商家还不能进行配置
 *   他用型B/S应用 -- 需要写入回调URL
 *   自用B/S型应用 -- 绑定一个沙箱账号

 * TODO:
 *   appkey -- 21540635
 *   sessionkey -- df1c85283e25dedf3df254c933a6c4c0
 *   secret -- MD5 加密
 *   code -- 应用序列号，对于单机应用，传入NULL即可，对于需要部署在多台机器的应用，必须传入与每个用户的序列号，且不能为NULL，不同用户的序列号不能相同
 *   userid -- 
 *   
 *   淘宝客
 *     商品数字id串
 *     pid -- 用户的pid,必须是mm_xxxx_0_0这种格式中间的"xxxx". 注意nick和pid至少需要传递一个,如果2个都传了,将以pid为准,且pid的最大长度是20。第一次调用接口的用户，推荐该入参不要填写，使用nick=（淘宝账号）的方式去获取，以免出错。
 *
 *   安全等级 ？
 *
 * 网页调用
 *   1.前台加入：<script src="http://l.tbcdn.cn/apps/top/x/sdk.js?appkey=21540635"></script>
 *   2.后台调用:   
 *
 * 淘宝开放平台(TOP -- Taobao Open Platform) http://open.taobao.com/doc/index.htm?spm=0.0.0.0.w4dIyE
 *  链接转换： http://api.taobao.com/apidoc/apiSample.htm?apiId=21417
 *             http://open.taobao.com/support/question_detail.htm?spm=0.0.0.0.itaPLq&id=128739
 *  在线转换工具：http://u.alimama.com/union/spread/activities/linksTrans.htm?spm=0.0.0.0.ATC3OP
 * 
 *    taobao.taobaoke.widget.items.convert
 *    taobao.taobaoke.widget.shops.convert
 *    
 * 
 * Windows 下的 TopClientSdk -- 提高客户端应用安全性，保证客户端应用的请求均从客户端发起，从而提高授权的安全等级(2级)
 *   返回值：返回的数据格式是 json 或 xml 的 ?
 *   接口:
 *     authorize(授权) -- 产生top授权中心访问地址(包括私有协议信息),相当于得到 ticket ?
 *     refresh(刷新) -- 刷新授权时长，对于安全等级为2的应用，每3天刷新一次，可以延迟R2时长
 *     call(调用Top) -- ?
 *     上传文件 -- content_type参数为Content-Type:multipart/form-data;charset=UTF-8;boundary=12345678

 *   系统分配AppKey只能调用基础API，增值API需要填入自己申请的AppKey
 *     Insufficient isv permissions -- 权限不足，需要申请增值权限
 *   isv.permission-call-from-user-only-limit -- 只有皇冠才能调用淘宝客API 
 *   api:taobao.taobaoke.widget.items.convert only support call from end user -- 只能从浏览器调用
 *   
 *   ITopClient client = new DefaultTopClient(url, appkey, appsecret)
 *     url
 *       沙箱(http://gw.api.tbsandbox.com/router/rest)
 *   	实际(http://gw.api.taobao.com/router/rest)
 *   ShopGetRequest req = new ShopGetRequest();
 *   req.Fields = "sid,cid,title,nick,desc,bulletin,pic_path,created,modified";
 *   req.Nick = "扣子北鼻";
 *   ShopGetResponse response = client.Execute(req);
 *   TextBox1.Text = response.Body;
 *   
 *   http://detail.tmall.com/item.htm?id=23706576785
 *   
*******************************************************************************************************************/

public class TopStudy
{
    public TopStudy()
    {
    }
}
