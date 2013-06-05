/*******************************************************************************
Go让函数很容易成为非常轻量的线程，线程之前的通讯使用 channel 完成
  goroutine--是Go并发能力的核心要素，并行执行，是轻量的(仅比分配栈多一点消耗),
    初识时栈很小，且随着需要在堆空间上分配

    TODO:没有任何办法知道，当所有goroutine都已经退出应当等待多久

通讯序列化过程(Communicating Sequential Processes - CSP)
channel--通信机制(双向的生产者消费者队列?)， 默认上是阻塞，可以用 select 来侦听多个channel
		 用于向系统中其他代理发送和接收特定类型对象的通信机制。
         可用于本地进程间通信(是否可用于分布式通信？)，用于连接到命名的目的地的库方法。
	ci := make(chan string [,缓冲大小])   //创建一个可以收发string的channel,可选设置缓冲大小，默认无缓冲
    ci <- myValue	//将变量myValue发送到channel ci中
	myValue := <-ci	//从channel ci 中读取变量，并保存到变量myValue中
	默认是阻塞方式读写，也可以通过 myValue, ok := <-ci 来非阻塞方式读写(需要判断ok变量)

使用 close 关闭，使用 closed 判断关闭状态？
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
	//"time"
	"runtime"
)

//首先定义会使用到的能收发int的channel -- 函数中的参数可以定义为 func xxx(cInput chan int)
var cInput chan int
var cResult chan int

func someResumerProc(i int) {
	//time.Sleep(i * time.Second)
	nInput := <-cInput //从channel接收整数，并保存到局部变量nInput中
	fmt.Printf("parallen routine %d will quit\n", nInput)

	cResult <- i //每个routine退出前都会向结果channel写一个值表示自己即将退出
}

const (
	PARALLEL_COUNT     = 3
	PARALLEL_TOTAL_SUM = 6 // = 1 + 2 + xxxx + PARALLEL_COUNT
)

func TestParallel(t *testing.T) {
	//创建发送和接收整数的输入channel，一个用于给消费者传递参数，一个用于等待所有的消费者结束
	cResult = make(chan int, PARALLEL_COUNT) //缓冲大小为PARALLEL_COUNT
	cInput = make(chan int)                  //无缓冲(等价于 ,0)

	//默认同一时刻只有一个goroutine执行，本函数设置可以并行的数量,也可以设置环境变量 GOMAXPROCS
	runtime.GOMAXPROCS(4)

	fmt.Printf("Test Parallel -- Before start goroutine\n")

	for i := 1; i <= PARALLEL_COUNT; i++ {
		go someResumerProc(i)
		cInput <- i //发送整数 i 到 channel cInput
	}

	nTotalCount := 0
	//*
	//使用两种方式检测所有的routine都退出的时机
	//方案1：直接使用 <- 读取channel
	for {
		nTmp := <-cResult //从结果channel中读取goroutine返回的数据
		fmt.Printf("Read %d from channel\n", nTmp)
		nTotalCount += nTmp
		if PARALLEL_TOTAL_SUM == nTotalCount {
			//所有的routine都退出
			break
		}
	}
	/*/
		//方案2：使用 select 来读取channel(select的使用场景是需要同时读取多个channel，此处仅是实例)
	CheckQuit:
		for {
			select {
			case nTmp := <-cResult:
				nTotalCount += nTmp
				if PARALLEL_TOTAL_SUM == nTotalCount {
					break CheckQuit
				}
			//case xxx := <-cOther:
			}
		}
		//*/

	fmt.Printf("Test Parallel -- After all goroutine ends\n")
}
