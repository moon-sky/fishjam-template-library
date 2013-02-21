Test.js 有用于计算长整形相加的方法，不过好像有些问题了（特别长的字符串转换成整数时会有溢出错误）
TestAdd.html是一个trace的简单示例。
Trace.html 是显示trace输出信息的窗口，其中有clear、Copy功能

trace.js 是用于日志输出的文件，使用其中的trace* 函数进行日志输出。
其中 
trace() 简单输出字符串；
traceObj() 输出类及其不为空的属性；
tracefmt() 采用格式化输出（目前只支持%s参数，testAdd.html 中有例子）
traceHTML() 输出所在网页的 outerHTML 

存在问题：
  由于无法对trace输出窗口进行跟踪，如果关闭输出窗口后重新进行trace跟踪会报错。

有其他问题再联系
