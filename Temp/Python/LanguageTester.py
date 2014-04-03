#! /usr/bin/env python
#coding=utf-8

# PYTHONPATH 环境变量
import unittest   #单元测试框架
from test import support

import os

#logger -- logging,可帮助实现灵活的日志系统(紧急/错误/警告/信息/调试)
#profile工具 -- profile/hotshot/cprofile (差 -->好)
#高级数字科学计算可使用:NumPy(numeric.scipy.org)和SciPy(scipy.org)

# Python优化选项被打开（生成“.pyo"字节码文件），assert语句将会被跳过
# 疑问：
#   文件打开后未关闭 -- 会造成资源泄漏，如何检测？
#
# 注意：
#   Python自动管理变量的内存，但可通过 del 关键字直接释放资源(强制引用计数减一)
#   Python不支持函数重载
#   for循环可遍历所有的可迭代类型
#   copy模块负责处理对象的浅拷贝(copy.copy)和深拷贝(copy.deepcopy),默认为浅拷贝

###############################################################################
# 反斜线(\)连续上一行;
# 三个单引号包括下的字符串可以跨行书写;
# 分号(;)将两个语句连在一行
# 冒号(:)将代码块的头和体分开，如 if,while,def 和 class 等
# 不同的缩进深度分割不同的代码块
# 编码风格：缩进用4个空格宽度，避免使用制表符，防止代码逻辑或语法错误
# Python中对象是通过引用传递的，赋值时只是将对象的引用赋值给了变量
# 空对象 -- 值为零的任何数字或NULL对象None德布尔值都是 False
# 整数支持取反(~),按位与(&),或(|),异或(^),左移(<<),右移(>>)
# 代码对象--编译过的python源代码片段，是个可执行对象，可通过内建函数 compile 获得，
#   可被 exec 或 eval 内建函数来执行
# 帧对象--执行栈帧，每次函数调用产生一个新的帧
# 工厂函数--产生该类型的实例，2.2以前是转换函数，如 int(),str(),unicode(),list()等
###############################################################################
 

###############################################################################
# PEP -- Python Enhancement Proposal,Python增强提案，在新版python中增加新特性的方式
#
#
# 断言(AssertionError) -- assert 条件, '失败的字符串'
# 钩子函数(hook method)-- 就是虚函数?
# 资源管理：self.widget.dispose() {CR} self.widget = None
# 方法、类的 doc-string 文档,通常用三个双引号包含起来
# 模块 -- 组织形式，将有关的Python代码组织到独立文件中，模块名即文件名(不带.py),
#   另一模块中用 import 导入使用，使用时的方式为： 模块名.函数名()
#   还有一种 from xxx import yyy 的语法 -- 然后可直接使用？
#
# 定义类
#   class 类名(父类): 
#     def __init__(self[,构造参数]):
#        self.变量名 = 初始化        
#     def 成员函数名(self):
#     def 静态函数名: ???
#
# 
###############################################################################

class PythonLanguageTester(unittest.TestCase):
    '''Python Language Tester'''   #此处写的是Class的注释，通过__doc__可访问文档字符串
    
    def setUp(self):
        print('in PythonLanguageTester setUp')
        
    def tearDown(self):
        print("in PythonLanguageTester tearDown")
        
    '''Python Language Fun Tester'''
    def test_language_fun(self):
        self.assertEqual(self.__class__.__name__, "PythonLanguageTester")
        self.assertEqual(self.__doc__, "Python Language Tester")
        self.assertEqual(self.__class__.__doc__, "Python Language Tester")
        
        # 切片对象(按索引访问对象元素)--使用python扩展的切片语法时创建,也可由内建的slice函数生成
        #   sequence[起始索引:结束索引:步进值], 如 forstr='abcde', forstr[::-1]会倒置
        forstr='abcdef'
        self.assertEqual(forstr[::-1], 'fedcba')     
        
    def test_ClassProperty(self):
        # 判断变量是否指向相同的元素，使用 is 关键字, a is b 等价于 id(a)==id(b)
        a = (1,2,3)
        b = (2,3,4)
        self.assertNotEqual(id(a), id(b))
        self.assertTrue( a is not b)
        b = a
        self.assertEqual(id(a), id(b))
        self.assertTrue(a is b)
        
        #isinstance(对象，类别如int/long/float/complex)--判断是否是其中的一个实例
        
        #python会缓存 简单整数和字符串(只读的不可变对象),赋值时会使用已有的变量
        str1 = "fishjam"
        str2 = "fishjam"
        self.assertEqual(id(str1), id(str2))
        self.assertTrue( str1 is eval(repr(str1)))
        #print(unicode('我','gb2312'))
        
    #/////////////////////////////////////////////////////////////////////////////#
    # 常用函数
    #   chr(单字节整数)--返回对应字符
    #   ord(字符)--返回对应整数
    #   unicode()--转换成Unicode字符串
    #   unichr(Unicode的码值)--返回对应的Unicode字符，所接收的码值范围依赖于UCS-2或UCS-4
    #   dir(对象)--显示对象的属性，如无参数，显示全局变量的名字
    #   help(对象)--显示对象的文档字符串
    #   id(obj)--返回对象唯一标识，可认为是内存地址
    #   int() -- 类型转换，早期版本是 string.ato*()
    #   range([start,] stop [,step])--返回整数列表,[start,stop-1]
    #   str()--返回对象适合可读性好的字符串表示
    #   type()--返回对象类型(import types可看各种类型)
    #   
    #   cmp(obj1,obj2) -- 比较对象，自定义类时会调用其 __cmp__ 方法
    #   repr(obj) 或 `obj`(旧方法) -- 返回对象的字符串表示，通常可重新获得该对象，
    #     即 obj=eval(repr(obj))
    #
    # 全局变量
    #
    # 容器 -- 
    #   序列(list,tuple,string等)每个元素可通过偏移量的方式得到，通过切片(slice)得到多个元素
    #     成员关系操作符(in,not in)返回True(在序列中)或False，
    #     索引范围为 0~len(seq)-1 和 (-len(seq))~(-1)
    #     
    #   array(数组)--高效的数值数组
    #   list
    #   tuple(元组)--(a,b,c)
    #   random--多种伪随机数生成器
    #\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\#
    def test_GeneralFunctions(self):
        x=1
        y=2
        (x,y) = (y,x) #通过元组赋值的方式交换两个变量的值
        self.assertTrue(isinstance((x,y), tuple ))  #元组 -- tuple
        self.assertEqual(x, 2)
        self.assertEqual(y, 1)
        self.assertTrue(isinstance(dir(self),list))
        
        #区分操作系统(Dos,Unix)的换行符
        self.assertEqual(os.linesep, '\r\n')  
        

class IntegerArithmenticTestCase(unittest.TestCase):
    def setUp(self):
        print("in IntegerArithmenticTestCase setUp")
        
    def tearDown(self):
        print("in IntegerArithmenticTestCase tearDown")
        
    def testAdd(self):  ## test method names begin 'test*'
        print("in IntegerArithmenticTestCase testAdd")
        self.assertEqual((1 + 2), 3)
        self.assertEqual(0 + 1, 1)
        
    def testMultiply(self):
        print("in IntegerArithmenticTestCase testMultiply")
        self.assertEqual((0 * 10), 0)
        self.assertEqual((5 * 8), 40)

#组装 测试套件(test suite), 可被嵌套 -- alltests = unittest.TestSuite((suite1, suite2))
def suite():
    #创建一个由测试用例类内所有测试用例组成的测试套件
    #suite = unittest.TestSuite()
    #suite.addTest(PythonLanguageTester())
    #suite.addTest(IntegerArithmenticTestCase())
    suite = unittest.makeSuite(IntegerArithmenticTestCase,'test')
    return suite

#通常比较全局变量 __name__的值，来判断模块是被导入(模块名)还是直接运行(__main__)
#单独运行这个文件时，自动加载并运行所有的测试用例,模块实现中通常会调用测试代码
if __name__ == "__main__":
    unittest.main()
    
