 var tracewin = null;
 var tracenum=1;
 function trace(str){
   if(tracewin==null || tracewin.closed)
     {
      tracewin = window.open('trace.html','tracewindow',
        'height=400;width=100;top=600;left=800;resizable=yes');
     }
   if(tracewin){
        tracewin.traceStr(tracenum+":"+str);
       tracenum++;
   }
}

function traceObj(obj){
  //return;
    var strObj='obj:'+obj+'\n\r';
    for(prop in obj)
    {
        if(obj[prop])
            strObj += ('\t'+prop +'='+obj[prop]+';\n\r');
    }   
    trace(strObj);
    return strObj;
}

function format()
{
  var i, msg = "", argNum = 0, startPos;
  var args = format.arguments;
  var numArgs = args.length;
  if(numArgs)
  {
    theStr = args[argNum++];
    startPos = 0;  endPos = theStr.indexOf("%s",startPos);
    if(endPos == -1) endPos = theStr.length;
    while(startPos < theStr.length)
    {
      msg += theStr.substring(startPos,endPos);
      if (argNum < numArgs) msg += args[argNum++];
      startPos = endPos+2;  endPos = theStr.indexOf("%s",startPos);
      if (endPos == -1) endPos = theStr.length;
    }
    if (!msg) msg = args[0];
  }
  return msg;
}

function tracefmt()
{
  var i, msg = "", argNum = 0, startPos;
  var args = tracefmt.arguments;
  var numArgs = args.length;
  if(numArgs)
  {
    theStr = args[argNum++];
    startPos = 0;  endPos = theStr.indexOf("%s",startPos);
    if(endPos == -1) endPos = theStr.length;
    while(startPos < theStr.length)
    {
      msg += theStr.substring(startPos,endPos);
      if (argNum < numArgs) msg += args[argNum++];
      startPos = endPos+2;  endPos = theStr.indexOf("%s",startPos);
      if (endPos == -1) endPos = theStr.length;
    }
    if (!msg) msg = args[0];
  }
  trace(msg);
}

function traceHTML(){
  var parObj = document.body;
  while(parObj && parObj.tagName != 'HTML'){
    parObj = parObj.parentElement;
  }
    trace(parObj.outerHTML);
}

function getTime(){
  var now= new Date();
  var strTime= now.toLocaleTimeString()+':'+ (now%1000);
  return strTime;
}
function traceTime(info){
  var strInfo = getTime()+':'+ info;
  trace(strInfo);
}