/*******************************************************************************
array是值类型的--将数组赋值给另一个数组，会复制所有的元素，需要注意函数参数是数组时的性能损失
slice是引用类型--但因为slice底层是固定长度的array，通过append追加新值时，会生成新的实例，
  使用其子范围的数据生成新的slice，是否会拷贝内存来生成新的实例?

slice/channel/map/interface 等都是引用类型
自定义的 struct 是值传递。所以在定义其成员方法时，需要使用指针 func (r *Rect) Area() float64
  但注意：虽然 s 是指针，但引用其变量时仍然用点(参见 Rect::Area )

golang.org/s/load-balancer
https://talks.golang.org/2013/advconc.slide#1
http://www.open-open.com/lib/view/open1352264498172.html
*******************************************************************************/

package gostudy

import (
	"fmt"
	"math/rand"
	"testing"
	"time"
)

func TestPerformance(t *testing.T) {

}

//type Result struct {
//	result string
//}
type Result string
type Search func(query string) Result

func firstRespFromServer(query string, replicas ...Search) Result {
	c := make(chan Result)
	searchReplica := func(i int) { c <- replicas[i](query) }
	for i := range replicas {
		go searchReplica(i)
	}
	return <-c
}

func fakeSearch(kind string) Search {
	return func(query string) Result {
		time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond)
		return Result(fmt.Sprintf("%s result for %q\n", kind, query))
	}
}

//测试 在负载均衡的服务器群中执行任务，将任务交给多个服务器执行，并返回第一个响应
func TestSearchOnReplicateServer(t *testing.T) {
	fmt.Printf("Enter TestSearchOnReplicateServer\n")
	rand.Seed(time.Now().UnixNano())
	start := time.Now()
	result := firstRespFromServer("golang",
		fakeSearch("replica 1"),
		fakeSearch("replica 2"))
	elapsed := time.Since(start)

	fmt.Println(result)
	fmt.Println(elapsed)

	fmt.Printf("Leave TestSearchOnReplicateServer\n")
}
