function onMyEnumWindow() {
    //alert("onMyEnumWindow");
    var WindowMediator = Cc["@mozilla.org/appshell/window-mediator;1"]
    .getService(Ci.nsIWindowMediator);

    var enumerator = WindowMediator.getEnumerator(null);
    if(enumerator.hasMoreElements()) {
        var win = enumerator.getNext().QueryInterface(Ci.nsIDOMWindowInternal);
        trace(win.Cc);
        //traceObj(win);
        //traceObj(win);
    }
    
    //var browser = WindowMediator.getMostRecentWindow('navigator:browser');
    //alert(browser.gBrowser.mTabs.length);
    //l.value = browser.gBrowser.mTabs.length;
}

//将一个本地文件路径转换为URL
function transLocalPathToURL(path) {
    // C:\\temp\\temp.txt =>  "file:///C:/temp/temp.txt"
    var file = Cc['@mozilla.org/file/local;1'].createInstance(Ci.nsILocalFile);
    file.initWithPath(path);
    var ioService = Cc['@mozilla.org/network/io-service;1'].getService(Ci.nsIIOService);
    var url = ioService.newFileURI(file);
    var fileURL = url.spec;
    return fileURL;
}

//将一个URL 转换为本地文件路径
function transURLToLocalPath(url) {
    // "file:///C:/temp/temp.txt" =>  "C:\\temp\\temp.txt"
    var ioService = Components.classes['@mozilla.org/network/io-service;1'].getService(Components.interfaces.nsIIOService);
    var fileHandler = ioService.getProtocolHandler('file').QueryInterface(Components.interfaces.nsIFileProtocolHandler);
    var file = fileHandler.getFileFromURLSpec(url);
    var path = file.path;
    return path;
}

//一旦过程完成，应该关闭所有的流。
function testReadBinLocalFile(path) {
    var file = Cc['@mozilla.org/file/local;1'].createInstance(Ci.nsILocalFile);
    file.initWithPath(path);
    //file.create(file.NORMAL_FILE_TYPE, 0666);   //如果是要写文件，需要使用 create 方法创建
    var fileStream = Components.classes['@mozilla.org/network/file-input-stream;1'].createInstance(Components.interfaces.nsIFileInputStream);

    //当我们初始化nsIFileInputStream 的时候，我们将第二个和第三个参数初始化为只读模式。
    //fileStream.init(file, 2, 0x200, false);   //写文件时将第二和第三个参数初始化为只写模式
    fileStream.init(file, 1, 0, false);
    
    var binaryStream = Components.classes['@mozilla.org/binaryinputstream;1'].createInstance(Components.interfaces.nsIBinaryInputStream);
    binaryStream.setInputStream(fileStream);

    //注意：MDN中的函数原型为 -- void readByteArray(in PRUint32 aLength, [array, size_is(aLength), retval] out PRUint8 aBytes);
    var array = binaryStream.readByteArray(fileStream.available());
    
    binaryStream.close();
    fileStream.close();
    
    alert(path +" = " + array.map(
        function(aItem) { return aItem.toString(16); }
        ).join(' ').toUpperCase()
    );
}

function testWriteTextLocalFile(strInfo, filePath) {
    var file = Cc['@mozilla.org/file/local;1'].createInstance(Ci.nsILocalFile);
    file.initWithPath(filePath);
    if (file.exists()) {
        file.remove(false);
    }
    
    file.create(file.NORMAL_FILE_TYPE, 0666);
    var charset = "intl.charset.default"; //"UTF-8"; //"GB2312";
    var fileStream = Cc['@mozilla.org/network/file-output-stream;1'].createInstance(Ci.nsIFileOutputStream);
    fileStream.init(file, 2, 0x200, false);
    var converterStream = Cc['@mozilla.org/intl/converter-output-stream;1'].createInstance(Ci.nsIConverterOutputStream);
    converterStream.init(fileStream, charset, strInfo.length, converterStream.DEFAULT_REPLACEMENT_CHARACTER);
    converterStream.writeString(strInfo);
    converterStream.close();
    fileStream.close();
    
    alert("write " + filePath + " OK");
}