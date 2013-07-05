/*******************************************************************************
html
  EscapeString -- 转换?

net -- 支持基于IP层(RawSocket),tcp/udp 层及更高层面(HTTP/FTP/SMTP/)的网络操作
  Dial("协议", "地址[:端口]") -- 客户端连接服务器，返回 Conn
    支持协议:tcp/tcp4/tcp6/udp/udp4/udp6/ip/ip4/ip6/ip4:icmp 等
    是 DialTCP/DialUDP/DialIP/DialUnix 等的封装
  ResolveXxxAddr -- 解析指定协议的地址
  LookupHost --
  ParseIP -- 验证IP地址有效性
  IPv4Mask/DefaultMask -- 创建/获取默认 子网掩码

  Conn
    Write()
    Read()

net/http -- 提供了HTTP客户端和服务端的功能,默认时通过 http.DefaultClient 调用，可通过 http.Client 定制
  Post("url", "image/jpeg", &imageDataBuf) -- 上传图片，判断resp.StatusCode==http.StatusOK
  PostForm -- 实现了标准编码格式为 application/x-www-form-urlencoded 的表单提交，如:
    PostForm("url", url.Values{"title":{"article title"}, "content":{"article body"}})
  Head() -- 值请求目标URL的头部信息

  Client.Do() -- 自定义PostArgument字段(如传递cookie)进行Post
  NewRequest() -- 返回Request对象，然后可以执行 Header.Add 等自定义Header，然后通过Do发送

  定制http.Client( myClient:=&http.Client{xxxx} )
    Transport RoundTripper -- 指定了执行一个HTTP请求的运行机制
    CheckRedirect func(xxx) -- 指定处理重定向的策略，若相应状态码为30x(301/302/303/307等)时，
      在遵循跳转规则之前会先调用这个函数
    Jar CookieJar -- 用于设定Cookie。但一般都用 http.SetCookie() 方法来设定

  服务器端(如想更多的控制，可自定义Server -- myServer:=&http.Server{ xxxx } )
    ListenAndServe() -- 在指定第地址进行监听，开启一个HTTP服务，第二个参数通常为nil(通过HandleFunc分别指定多个处理程序)
	ListenAndServeTLS() -- 处理HTTPS连接请求
    HandleFunc() -- 针对某一指定路径请求，将其映射分发到指定的业务逻辑处理方法中
    StatusInternalServerError -- 内部错误码

net/rpc(Remote Procedure Call)--允许客户端通过网络或其他IO调用一个远端对象的公开方法(大写字母开头)，
  服务端：将一个对象注册为可访问的服务，之后该对象的公开方法就能够以远程的方式提供访问
    方法签名：func (t *T) MethodName(inputArg T1, replyResult* T2) error
	ServeConn -- 服务器端通过这种对象处理单个连接请求
  客户端:
    rpc.Dial 或 rpc.DialHTTP 与指定的RPC服务端建立连接，然后调用Call(同步)或Go(异步)请求处理结果
  自定义编解码器接口(默认使用gob，只能用于go)，比如 jsonrpc ?
    type ClientCodec interface { WriteRequest/ReadRespponseHeader/ReadResponseBody/Close 等 }
	type ServerCodec interface { WriteResponse/ReadRequestHeader/ReadRequestBody/Close 等}
*******************************************************************************/
package gostudy

import (
	"fmt"
	"net"
	"net/http"
	"net/rpc"
	"testing"
)

func serverProc() {

}

func clientProc() {
	//net.()
	//var conn net.Conn = nil

	if conn, err := net.Dial("tcp", "127.0.0.1:5678"); err == nil {
		defer conn.Close()
		//return
	}

}
func TestNet(t *testing.T) {
	fmt.Printf("Test Tools")

}

type RectRpc Rect

func (rRpc *RectRpc) Area(r *Rect, reply *float64) error {
	*reply = r.Height * r.Width
	return nil
}

func TestRpc(t *testing.T) {
	//注册一个RectRpc类型的对象及其公开方法 RectRpc.Area 供客户端调用
	rRpc := new(RectRpc)
	rpc.Register(rRpc)
	rpc.HandleHTTP()

	listener, err := net.Listen("tcp", ":1234")
	GOUNIT_ASSERT(t, err == nil, "Rpc 服务器监听")

	go http.Serve(listener, nil)

	//RPC客户端
	client, err := rpc.DialHTTP("tcp", "127.0.0.1:1234")
	GOUNIT_ASSERT(t, nil == err, "Rpc客户端连接")
	args := &Rect{5.0, 10.0}
	var result float64

	//同步调用
	err = client.Call("RectRpc.Area", args, &result)
	GOUNIT_ASSERT(t, nil == err, "Rpc客户端同步调用")
	GOUNIT_ASSERT(t, isEqual(result, 50.0, 0.0001), "Rpc客户端同步调用结果")

	//异步调用
	result = 0.0
	args.Height = 5.0
	//asyncCall := client.Go("RectRpc.Area", args, &result, nil)
	//result = <-asyncCall.Done
	//GOUNIT_ASSERT(t, isEqual(result, 25.0, 0.0001), "Rpc客户端异步调用结果")
}
