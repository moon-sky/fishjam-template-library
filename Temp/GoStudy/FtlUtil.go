package gostudy

//import (
//"fmt"
//)

//使用switch对字节数组进行比较的例子：
//  比较返回两个字节数组字典顺序先后的整数，如果a==b返回0,如果a<b返回-1,如果a>b返回+1
func MyCompareBytes(a, b []byte) int {
	for i := 0; i < len(a) && i < len(b); i++ {
		switch {
		case a[i] > b[i]:
			return 1
		case a[i] < b[i]:
			return -1
		}
	}
	//长度不同，则不相等
	switch {
	case len(a) < len(b):
		return -1
	case len(a) > len(b):
		return 1
	}
	return 0
}

//使用不定参数(变参)的函数
func MySum(nums ...int) int {
	var sum int = 0
	for _, num := range nums { //nums 是一个 int 的 slice
		sum += num
	}
	//在函数体内可以通过 MyFun2(nums...)或MyFun2(num[:2]...) 的方式调用其他变参函数
	return sum
}

//计算slice的平均值
func MyAverage(xs []float32) (ave float32) {
	sum := float32(0.0)
	switch len(xs) {
	case 0:
		ave = 0 //如果长度是0，返回0(避免除以0)
	default:
		for _, v := range xs {
			sum += v
		}
		ave = sum / float32(len(xs)) //必须转换成 float32/float64
	}
	return ave
}

//转换16进制的字符为对应的整数
func UnHex(c byte) byte {
	switch {
	case '0' <= c && c <= '9': //文档中说这两种方法等价，但实测不正确，Why？
		//case '0', '1', '2', '3', '4', '5', '6', '7', '8', '9':
		return c - '0'
	case 'a' <= c && c <= 'z':
		return c - 'z' + 10
	case 'A' <= c && c <= 'Z':
		return c - 'A' + 10
	default:
		return 0
	}
}

//生成全局的函数映射
var funMaps = map[int]func() string{
	1: func() string { return "one" },
	2: func() string { return "two" },
}

//定义 int->func() 的map(演示函数映射的用法)
//  根据传入的int，找到对应的匿名函数并执行，返回对应的字符串
func NumberConverter(num int) (name string) {
	if pFun, bPresent := funMaps[num]; bPresent {
		name = pFun()
	} else {
		name = "unknown"
	}
	return
}

//计算斐波那契数列 -- 尚未调用
func Fibonacci(value int) []int {
	x := make([]int, value) //创建用于保存执行结果的slice
	x[0], x[1] = 1, 1
	for n := 2; n < value; n++ {
		x[n] = x[n-1] + x[n-2]
	}
	return x
}

//一个冒泡排序的实现, 调用: n:=[]int{ 2,-1, 0, 4, 5 }; BubbleSort(n);
func BubbleSort(n []int) {
	for i := 0; i < len(n)-1; i++ {
		for j := i + 1; j < len(n); j++ {
			if n[j] < n[i] {
				n[i], n[j] = n[j], n[i]
			}
		}
	}
}
