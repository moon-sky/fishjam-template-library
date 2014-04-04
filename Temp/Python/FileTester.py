#! /usr/bin/env python
#coding=utf-8

import unittest   #单元测试框架
from test import support
import file

class FileTester(unittest.TestCase):
    
    def setUp(self):
        print("File Tester setUp")
        
    def tearDown(self):
        print("File Tester tearDown")
        
    '''Python Language Fun Tester'''
    def test_File(self):
        #fObj = file
        #fobj.writelines([%s %s % (x, ls) for x in all]) #列表解析
        self.assertEqual(1+2,3)

def suite():
    #suite = unittest.TestSuite()
    #suite.addTest(FileTester())
    suite = unittest.makeSuite(FileTester,'test')
    return suite

#单独运行这个文件时，自动加载并运行所有的测试用例
if __name__ == "__main__":
    unittest.main()
