#! /usr/bin/env python
#coding=utf-8



#正则表达式 re, 有search('key','字符串'), 通常要用原是字符操作符(r?)来防止转义

import unittest   #单元测试框架
import repr

class RegTester(unittest.TestCase):
    
    def setUp(self):
        print("Reg Tester setUp")
        
    def tearDown(self):
        print("Reg Tester tearDown")
        
    def test_Reg(self):
        #os.path.exists
        self.assertEqual(1+2,3)

def suite():
    #suite = unittest.TestSuite()
    #suite.addTest(FileTester())
    suite = unittest.makeSuite(RegTester,'test')
    return suite

#单独运行这个文件时，自动加载并运行所有的测试用例
if __name__ == "__main__":
    unittest.main()
