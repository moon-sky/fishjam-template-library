package main

import (
	"fmt"
	"net"
	//"os"
	"bytes"
	"io"
	"runtime"
	"strconv"
	"time"
)

func Usage() {
	fmt.Printf("Usage: xxxx\n")
}

//从Conn中读取全部数据 -- 系统已经提供了 ioutil.ReadAll ?
func readFully(conn net.Conn) ([]byte, error) {
	//defer conn.Close()

	result := bytes.NewBuffer(nil)
	var buf [512]byte
	for {
		n, err := conn.Read(buf[0:])
		result.Write(buf[0:n])
		if err != nil {
			if err == io.EOF {
				break
			}
			return nil, err
		}
	}
	return result.Bytes(), nil
}

func scanServerPort(server, port string) error {
	strServer := fmt.Sprintf("%s:%s", server, port)
	fmt.Printf("scanServerPort for %s\n", strServer)

	conn, err := net.Dial("tcp", strServer)
	fmt.Printf("scan %s, result is %v\n", strServer, err)
	if err != nil {
		return err
	}
	defer conn.Close()
	_, err = conn.Write([]byte("HEAD / HTTP/1.0\r\n\r\n"))

	//var buf [512]byte
	//n, _ := conn.Read(buf[0:len(buf)])
	result, err := readFully(conn)
	fmt.Printf("Read -- %s\n", string(result))
	return nil
}

func scanServer(server string, ports []int) error {

	for _, port := range ports {
		go scanServerPort(server, strconv.Itoa(port))
	}
	return nil
}

func main() {
	//if len(os.Args) <= 2 {
	//	Usage()
	//	os.Exit(1)
	//}
	runtime.GOMAXPROCS(runtime.NumCPU())

	ports := make([]int, 20)
	for i := 0; i < len(ports); i++ {
		ports[i] = 70 + i
	}
	scanServer("www.baidu.com", ports)

	time.Sleep(time.Second * 100)
}
