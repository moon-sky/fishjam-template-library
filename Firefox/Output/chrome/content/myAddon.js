function ShowWin32APIMessage() {
    //参考 Youdao 插件的实现
    Components.utils.import("resource://gre/modules/ctypes.jsm");  
    var lib = ctypes.open("C:\\WINDOWS\\system32\\user32.dll");
     
    /* Declare the signature of the function we are going to call */  
    var msgBox = lib.declare("MessageBoxW",  
                             ctypes.winapi_abi,  
                             ctypes.int32_t,  
                             ctypes.int32_t,  
                             ctypes.jschar.ptr,  
                             ctypes.jschar.ptr,  
                             ctypes.int32_t);  
    var MB_OK = 0;  
    var ret = msgBox(0, "Hello world", "title", MB_OK);  

    lib.close();
}


function ShowNpAPIMessage()
{
    var obj = document.getElementById("mypluginobj");
    if (obj) {
        trace("scrollHeight=" + obj.scrollHeight);
         //traceObj(obj);
        
        //trace(obj.tagName);
        
//        alert(obj.tagName);
//        alert(obj.toLocalString());
    }
    else
    {
        alert("cannot find mypluginobj");
    }
}
