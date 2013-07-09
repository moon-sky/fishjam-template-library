/*******************************************************************************
crypto -- 数据加密相关的部分
  cipher -- 实现了标准的密码块模式，该模式可包装进低级的块加密实现中
  rand -- 伪随机数发生器
  rsa -- 非对称加密算法
  tls -- 传输层安全协议
  x509 -- 一种常用的数字证书格式

问题：怎么计算文件的Hash等(内存问题)
*******************************************************************************/

package gostudy

import (
	"crypto/md5"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"testing"
)

const (
	strOriginal string = "hello world, this is unencrypted string"
)

func TestHashMd5(t *testing.T) {
	fmt.Println("TestCryptoMd5\n")

	md5Instance := md5.New()
	md5Instance.Write([]byte(strOriginal))
	md5Result := md5Instance.Sum([]byte(""))

	var strMd5Result string = "13d4a69ca8e02af455b2c974b4e5b4c3"
	//fmt.Printf("%x\n", md5Result)
	GOUNIT_ASSERT(t, (hex.EncodeToString(md5Result) == strMd5Result), "Hash_MD5")
	GOUNIT_ASSERT(t, len(strMd5Result) == 16*2, "MD5的结果是16个字节")
}

func TestHashSha1(t *testing.T) {
	sha1Instance := sha1.New()
	sha1Instance.Write([]byte(strOriginal))
	sha1Result := sha1Instance.Sum([]byte(""))

	var strSha1Result string = "4e4189f0aa4d6d319e00391d2ddd063c9a166025"
	//fmt.Printf("%x\n", sha1Result)
	GOUNIT_ASSERT(t, (hex.EncodeToString(sha1Result) == strSha1Result), "Hash_Sha1")
	GOUNIT_ASSERT(t, len(strSha1Result) == 20*2, "SHA1的结果是20个字节")

}
