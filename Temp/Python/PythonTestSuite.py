#! /usr/bin/env python
#coding=utf-8

###############################################################################
# PyUnit -- The Python unit testing framework(Python单元测试框架)
# unittest.TestCase -- 测试用例的父类，可有 setUp/tearDown 的钩子函数(hook method)
#   assertEqual/assertNotEqual
#   fail/failIf/failUnless
#   测试异常:try: 调用代码 {CR} except 指定异常: pass {CR} else: {CR} fail("失败信息")
#     也可用:assertRaises(期待异常, 可调用的对象 [, 可调用对象的参数] )
#
#   可通过覆盖 runTest 方法实现测试用例的运行
#   所有的名字为 test 开头的方法都是测试方法，会自动执行，执行前后会执行 setUp/tearDown
#   unittest.main() -- 使用 unittest.TestLoader 类来自动查找和加载模块内测试用例
###############################################################################

import unittest
#from test import support


#加载编写的各种测试套(TestSuite)集 
import LanguageTester
import FileTester
import OsTester

def test_main():
#    suite = unittest.TestLoader().loadTestsFromTestCase(LanguageTester)
#    unittest.TextTestRunner(verbosity=2).run(suite)
    allSuite = unittest.TestSuite(
        LanguageTester.suite()
        #,FileTester.suite()
        #,OsTester.suite()
        )
    unittest.TextTestRunner().run(allSuite)
    
    
#    support.run_unittest(
#        LanguageTester.IntegerArithmenticTestCase
#        )

if __name__ == "__main__":
    test_main()
