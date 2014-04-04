#! /usr/bin/env python
#coding=utf-8

import unittest   #单元测试框架
from test import support

#################################################################################
# Python.exe
#   file 文件名 从指定文件运行python脚本
#   命令行参数：
#      -d 调试输出
#      -O 生成优化的字节码
#      -v 详细输出
#      -m mod 将一个模块以脚本形式运行
#      -c cmd 运行以命令行字符串形式提交的python脚本
#   退出：Windows下 CTRL+Z； Linux 下 CTRL+D, help(quit)， 也可通过 sys.exit() 退出
#################################################################################

class ToolsTester(unittest.TestCase):
    '''Python Tools Tester'''   #此处写的是Class的注释，通过__doc__可访问文档字符串
    
    def setUp(self):
        print('in ToolsTester setUp')
        
    def tearDown(self):
        print("in ToolsTester tearDown")
        
    def test_Tools(self):
        #os.path.exists
        self.assertEqual(1+2,3)
        
def suite():
    suite = unittest.makeSuite(ToolsTester,'test')
    return suite

if __name__ == "__main__":
    unittest.main()        