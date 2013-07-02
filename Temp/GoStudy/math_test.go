/*******************************************************************************
math数学库
*******************************************************************************/

package gostudy

import (
	"fmt"
	"math"
	"testing"
)

func TestMath(t *testing.T) {
	fmt.Printf("in TestMath")

	sqrtResult := math.Sqrt(16)
	if sqrtResult != 4 {
		t.Error("Math.Sqrt")
	}
}
