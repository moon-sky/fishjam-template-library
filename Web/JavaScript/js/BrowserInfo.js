//收集浏览器的信息，其中可以使用 this.xxx 来声明和设置属性
function BrowserInfo()
{
    var agent = navigator.userAgent.toLowerCase();
    
    this.major = parseInt(navigator.appVersion); //主版本号
    this.minor = parseFloat(navigator.appVersion);//全版本号
    
    //是否 Netscape
    this.IsNetscape = ((agent.indexOf('mozilla')!=-1) &&
    ((agent.indexOf('spoofer')==-1) && 
    (agent.indexOf('compatible') == -1)));
    
    this.IsIE = (agent.indexOf("msie") != -1); //是否 IE
}

var gBrowserInfo = new BrowserInfo();