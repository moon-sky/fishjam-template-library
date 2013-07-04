/*******************************************************************************
json
*******************************************************************************/
package gostudy

import (
	"encoding/json"
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
}
