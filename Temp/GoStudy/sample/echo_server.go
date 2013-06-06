//服务器端程序
package main

import (
	"fmt"
	"io"
	"net"
)

const RECV_BUF_LEN = 1024

func main() {
	listener, err := net.Listen("tcp", "0.0.0.0:6666") //侦听在6666端口
	if err != nil {
		panic("error listening:" + err.Error())
	}
	fmt.Println("Starting the server")
	for {
		conn, err := listener.Accept() //接受连接
		if err != nil {
			panic("Error accept:" + err.Error())
		}
		fmt.Println("Accepted the Connection :", conn.RemoteAddr())
		go EchoServer(conn)
	}
}

func EchoServer(conn net.Conn) {
	buf := make([]byte, RECV_BUF_LEN)
	defer conn.Close()

	for {
		n, err := conn.Read(buf)
		switch err {
		case nil:
			conn.Write(buf[0:n]) //回写全部数据
		case io.EOF:
			fmt.Printf("Warning: End of data: %s \n", err)
			return
		default:
			fmt.Printf("Error: Reading data : %s \n", err)
			return
		}
	}
}
