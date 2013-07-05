//简单的web服务器, 测试时浏览器中输入 http://localhost/html/index.html
package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"path/filepath"
	"runtime/debug"
	"time"
)

//使用闭包避免程序运行时出错崩溃
//  safeHandler 接收业务逻辑处理函数作为参数, 调用该业务逻辑函数后，defer的匿名函数会执行并检查
//  业务处理函数中是否引发了 panic, 如果调用了且为一般性错误，则输出错误信息、记录日志并恢复运行。
func safeHandler(fn http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		defer func() {
			if err, ok := recover().(error); ok {
				http.Error(w, err.Error(), http.StatusInternalServerError)
				//或者输出自定义的错误页面:
				log.Println("WARN: panic in %v - %v", fn, err)
				log.Println(string(debug.Stack()))
			}
		}()
		fn(w, r)
	}
}

const http_root = "F:\\Fujie\\FJCODE_GOOGLE\\Web"

func main() {
	http.HandleFunc("/", rootHandler)
	http.HandleFunc("/view/", safeHandler(viewHandler))
	http.HandleFunc("/html/", safeHandler(htmlHandler))
	http.HandleFunc("/hello", helloHandler)
	err := http.ListenAndServe(":80", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err.Error())
	}
}

//读取一些HTTP的头
func rootHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "rootHandler: %s\n", r.URL.Path)
	fmt.Fprintf(w, "URL: %s\n", r.URL)
	fmt.Fprintf(w, "Method: %s\n", r.Method)
	fmt.Fprintf(w, "RequestURI: %s\n", r.RequestURI)
	fmt.Fprintf(w, "Proto: %s\n", r.Proto)
	fmt.Fprintf(w, "HOST: %s\n", r.Host)
}

func helloHandler(w http.ResponseWriter, r *http.Request) {
	io.WriteString(w, "Hello wolrd, Now is :"+time.Now().String())
}

//特别的URL处理
func viewHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "viewHandler: %s", r.URL.Path)
}

//一个静态网页的服务示例。（在http_root的html目录下）
func htmlHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Printf("htmlHandler: %s\n", r.URL.Path)
	filename := http_root + r.URL.Path
	fileext := filepath.Ext(filename)

	content, err := ioutil.ReadFile(filename)
	if err != nil {
		fmt.Printf("   404 Not Found!\n")
		w.WriteHeader(http.StatusNotFound)
		return
	}

	var contype string //= http.DetectContentType()
	switch fileext {
	case ".html", "htm":
		contype = "text/html"
	case ".css":
		contype = "text/css"
	case ".js":
		contype = "application/javascript"
	case ".png":
		contype = "image/png"
	case ".jpg", ".jpeg":
		contype = "image/jpeg"
	case ".gif":
		contype = "image/gif"
	default:
		contype = "text/plain"
	}
	fmt.Printf("ext %s, ct = %s\n", fileext, contype)
	w.Header().Set("Content-Type", contype)
	fmt.Fprintf(w, "%s", content)

	//http.ServeFile(w, r, filename)
}
