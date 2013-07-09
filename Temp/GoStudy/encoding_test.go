/*******************************************************************************
编解码 -- 只能编解码导出(即首字母大写)的字段?

binary -- 实现了在无符号整数值和字节串之间的转化，以及对固定尺寸值得读和写
gob -- Go专用的序列化数据结构的编解码工具，自解释的二进制方式，结果可用于网络传输，RPC等的默认编解码器
hex -- 实现了十六进制的编码和解码
json --
  Marshal -- 对一组数据进行JSON格式的编码(先判断是否实现了json.Marshaler接口，)
    如转化前的数据结构中有非空指针，将转化指针所指向的值
  Unmarshal -- 将JSON格式的文本解码为Go里面预期的数据结构
  NewDecoder/NewEncoder -- 进行流式读写
  解码未知结构的JSON数据 -- 解出的数据是 map[string] interface{} 结构

pem(Privacy Enhanced Mail) -- 非对称加密体系下，一般用于存放公钥和私钥的文件
*******************************************************************************/
package gostudy

import (
	//"encoding/gob"
	"encoding/hex"
	"encoding/json"
	//"encoding/pem"
	"fmt"
	"testing"
)

func TestJson(t *testing.T) {
	var rSrc Rect //:= Rect{ 10.0, 20. 0 } //定义矩形
	rSrc.Height = 10.0
	rSrc.Width = 20.0

	//对象属性转换成字符串 -- ? Json将对象Marshal成[]byte, 然后可以转换成字符串，方便传递
	b, err := json.Marshal(rSrc)
	strJsonResult := string(b)
	fmt.Printf("Json Marshal Rect Result is %s\n", strJsonResult)
	GOUNIT_ASSERT(t, err == nil, "Json Marshal")
	//{"Width":20,"Height":10}
	GOUNIT_ASSERT(t, strJsonResult == "{\"Width\":20,\"Height\":10}", "strJsonResult")

	//从字符串中恢复对象属性 -- ? 从字符串转换成的[]byte中 Unmarshal到对象的属性值中
	var rDst Rect
	err = json.Unmarshal([]byte(strJsonResult), &rDst)

	GOUNIT_ASSERT(t, err == nil, "Json Unmarshal")
	GOUNIT_ASSERT(t, isEqual(rDst.Height, rSrc.Height, 0.0001), "UnMarshal Height")
	GOUNIT_ASSERT(t, isEqual(rDst.Width, rSrc.Width, 0.0001), "UnMarshal Width")

	//解码未知结构的JSON数据
	//定义了一个Json数据的byte数组(假设是从网络传递而来)
	jsonUnknown := []byte(`{
		"Title": "Go语言编程",
		"Authors": ["许式伟", "吕桂华"],
		"IsPublished": true,
		"Price": 49.00
	}`)
	//fmt.Printf("jsonUnknown= %s\n", string(jsonUnknown))

	//定义一个空接口变量，作为解析结果容器，最终会是一个键值对的 map[string] interface{} 结构
	var rUnknown interface{}
	err = json.Unmarshal(jsonUnknown, &rUnknown)

	GOUNIT_ASSERT(t, err == nil, "解析未知结构的JSON数据")
	gobook, ok := rUnknown.(map[string]interface{})
	GOUNIT_ASSERT(t, ok, "解析的结果能城砖转型成 map[string]interface{} 结构")
	if ok {
		for k, v := range gobook {
			switch v2 := v.(type) {
			case string:
				fmt.Println(k, "is String ", v2)
				GOUNIT_ASSERT(t, k == "Title" && v2 == "Go语言编程", "解析出来的Title")
			case int:
				fmt.Println(k, "is int ", v2)
			case bool:
				fmt.Println(k, "is bool ", v2)
				GOUNIT_ASSERT(t, k == "IsPublished" && v2, "解析出来的IsPublished")
			case float64:
				fmt.Println(k, "is float64 ", v2)
				GOUNIT_ASSERT(t, k == "Price" && isEqual(v2, 49.0, 0.1), "解析出来的Price")
			case []interface{}:
				fmt.Println("->", k, "is an array:")
				for i, iv := range v2 {
					fmt.Println(i, iv)
				}
			default:
				fmt.Println("JsonUnMarsal -- ", k, "is another type not handle yet, value=", v2)
				GOUNIT_ASSERT(t, false, "解析到尚未处理的类型")
			}
		}
	}
}

func TestGob(t *testing.T) {
	//gob.NewEncoder(nil)
	//var buf [512]byte
	//dec := gob.NewDecoder(buf[0:])
	fmt.Printf("TestGob")
}

func TestHex(t *testing.T) {
	_, err := hex.DecodeString("1234567890abcdef")
	GOUNIT_ASSERT(t, err == nil, "Hex Decode String")
}
