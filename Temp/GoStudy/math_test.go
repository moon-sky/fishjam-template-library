/*******************************************************************************
math数学库
*******************************************************************************/

package gostudy

import (
	"fmt"
	"math"
	"testing"
)

func isEqual(f1, f2, p float64) bool {
	return math.Abs(f1-f2) < p
}

func TestMath(t *testing.T) {
	fmt.Printf("in TestMath")

	sqrtResult := math.Sqrt(16)
	if sqrtResult != 4 {
		t.Error("Math.Sqrt")
	}

	//浮点数比较
	var f1 float64 = 3.1415926
	var f2 float64 = math.Pi
	if !isEqual(f1, f2, 0.00001) {
		t.Error("比较浮点数")
	}
}
