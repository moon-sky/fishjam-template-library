/*******************************************************************************
Go语言编程上抄下来的使用chan模拟的简单的IPC框架，封装通信包的编码细节
TODO：语法不正确，测试不通过，很多需要的功能都没有
是否已有成熟的IPC框架？
*******************************************************************************/

package ipc

import (
	"encoding/json"
	"fmt"
)

type Request struct {
	Method string "method"
	Params string "params"
}

type Response struct {
	Code string "code" //TODO: 200表示正确 -- 本例子里和Request.Method一样
	Body string "body"
}

//用Server接口确定了以后要实现的业务服务器的统一接口 -- 使用chan模拟解决网络成的通信问题
type Server interface {
	Name() string
	Handle(method, params string) *Response
	//Handle(request string) string
}

type IpcServer struct {
	Server
}

func NewIpcServer(server Server) *IpcServer {
	return &IpcServer{server}
}

func (server *IpcServer) Connect() chan string {
	session := make(chan string, 0)
	go func(c chan string) {
		for {
			request := <-c
			if request == "CLOSE" { //关闭该连接
				break
			}
			var req Request
			err := json.Unmarshal([]byte(request), &req)
			if err != nil {
				fmt.Println("Invalid request format:", request)
			}
			resp := server.Handle(req.Method, req.Params)
			b, err := json.Marshal(resp)

			c <- string(b) //返回结果
		}
		fmt.Println("Session closed.")
	}(session)

	fmt.Println("A new session has been created successfully.")
	return session
}
