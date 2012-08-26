function ToolBarTraceFF() {
    //construcotr
    this._console = Components.classes["@mozilla.org/consoleservice;1"].getService(Components.interfaces.nsIConsoleService);
    alert("ToolBarTraceFF constructor");
}

ToolBarTraceFF.property = {
    init: function() {
    },
    
    uninit: function() {
    },

    trace: function(str) {
        _console.logStringMessage(str);
    },
    
    traceObj: function (obj) {
        var strObj = "";
        for (prop in obj) {
            try {
                if (obj[prop]) {
                    //strObj += ('\t' + prop + ';\n\r');
                    strObj += (prop + '=' + obj[prop] + ";"); // + ';\n\r');
                }
            }
            catch (err) 
            {
                var errText = err.toLocaleString() + "\n" + err.description; 
                _onerror(errText);
                //throw err;
            }
        }
        trace(strObj);
    }
};
