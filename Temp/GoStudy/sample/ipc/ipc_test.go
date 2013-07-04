package ipc

import (
	"fmt"
	"testing"
)

func GOUNIT_ASSERT(t *testing.T, bAssert bool, str string) {
	if !bAssert {
		t.Error(str)
	}
}

type EchoServer struct {
	//servers map[string]Server //多个通过名字区分的服务器
}

func (server *EchoServer) Handle(method, params string) *Response {
	fmt.Printf("EchoServer::Handle, method=%s, param=%s\n", method, params)

	var res *Response = &Response{method, ""}
	switch method {
	case "ECHO":
		res.Body = "ECHO:" + params
	case "TIME":
		res.Body = "2012-12-21"
	default:
		res.Body = "Unknown"
	}
	return res
}

func (server *EchoServer) Name() string {
	return "EchoServer"
}

func TestIpc(t *testing.T) {
	//servers := make(map[string]Server)
	echoServer := NewIpcServer(&EchoServer{ /*servers: servers*/})

	client1 := NewIpcClient(echoServer)
	client2 := NewIpcClient(echoServer)
	defer client1.Close()
	defer client2.Close()

	resp1, _ := client1.Call("ECHO", "From Client1")
	resp2, _ := client2.Call("TIME", "From Client2")

	fmt.Printf("TestIpc resp1=%v, resp2=%v\n", resp1, resp2)
	bResult1 := (resp1.Body == "ECHO:From Client1")
	bResult2 := (resp2.Body == "2012-12-21")

	GOUNIT_ASSERT(t, (bResult1 && bResult2), "IpcClient.Call")
}
