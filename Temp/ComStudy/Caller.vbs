'Dim obj as Object;
Set obj = CreateObject("ATLCom.MyMath")	'以ProgID启动组件
MsgBox "hello's Upper is " + vbCrLf + obj.Upper("hello") '只能调用自动化接口(IDispatch)的内容，HELLO
Set obj = Nothing			            '释放