#! /usr/bin/env python
#coding=utf-8

#! /usr/bin/env python
#coding=utf-8

import unittest   #单元测试框架
import os

class OsTester(unittest.TestCase):
    
    def setUp(self):
        print("Os Tester setUp")
        
    def tearDown(self):
        print("Os Tester tearDown")
        
    def test_Os(self):
        #os.path.exists
        self.assertEqual(1+2,3)

def suite():
    #suite = unittest.TestSuite()
    #suite.addTest(FileTester())
    suite = unittest.makeSuite(OsTester,'test')
    return suite

#单独运行这个文件时，自动加载并运行所有的测试用例
if __name__ == "__main__":
    unittest.main()
