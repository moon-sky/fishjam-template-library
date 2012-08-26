var tbTrace = {
    _init : Boolean = false,
    _appName: String = navigator.appName,
    _traceImpl: Object = null,

    init: function() {
        this._init = true;
        alert("tbTrace init + _appName=" + this._appName);
        if (this._appName == "Netscape") {
            this._traceImpl = new ToolBarTraceFF();
            alert("_traceImpl = " + this._traceImpl);
            traceObj(this._traceImpl);
            _traceImpl.init();
        }
    },
    uninit: function() {
        this._traceImpl = null;
    },

    trace: function(str) {
        if (!this._init) {
            this.init();
        }
        alert("in tbTrace.trace, _traceImpl=" + this._traceImpl);
        this._traceImpl.trace(str);
    },
};
