main();

function main()
{
  //var obj = new ActiveXObject("ATLCom.MyMath"); //以ProgID启动组件
  var obj = WScript.CreateObject("ATLCom.MyMath");
  WScript.Echo(obj.Upper("hello"));
}