function GetSupNum(num,digital){
  if(num.charAt(0)!="-" && num.charAt(0)!="+"){
    num = "+" + num;
  }
  var len = num.length;
  var signChar = num.charAt(0) == "-" ? "9" : "0" ;
  var result = signChar;
  var i;
  for(i = digital;i>=len;i--){
    result += signChar;
  }
  if(signChar == "0"){
    for(i =1; i<len ; i++){
      result += num.charAt(i);
    }
  }
  else{
    for(i = 1;i<len;i++){
      result += "" + (9 - parseInt(num.charAt(i)));
    }
    result = AddTwoNum(result,GetSupNum("1",digital),digital);
  }
  if(result.length>digital+1){
    result = result.substr(1);//for -0;
  }
  return result;
}

function AddTwoNum(num1,num2,digital){
  //alert("Add:"+num1 +";"+num2);
  var result ="";
  var i;
  var numAdded = 0;
  for(i= digital;i>=0;i--){
    var charNum1 = parseInt(num1.charAt(i));
    var charNum2 = parseInt(num2.charAt(i));
    var charNum3 = charNum1+charNum2 + numAdded;
    numAdded = parseInt(charNum3 /10);
    result = (charNum3 %10) +result;
  }
  if(numAdded!=0){
    result = numAdded +result;
  }
  //alert(result);
  return result;
}

function GetNumDisplay(num,digital){
  if(num.length>digital+1){
    num = num.substr(1);
  }
  var result = "";
  var sign ="";
  if(parseInt(num.charAt(0))>7){
    sign = "-";
    for(var i = 0;i<num.length;i++){
      result = result + (9- parseInt(num.charAt(i)));
    }
    result = AddTwoNum(result,GetSupNum("1",digital),digital);
  }else{
    result = num;
  }
  
  while(result.charAt(0)=="0"){
    result = result.substr(1);
  }
  return sign+result;
}

function LongNumAdd(num1,num2,digital){
  var result = AddTwoNum(GetSupNum(num1,digital),GetSupNum(num2,digital),digital);
  alert(result);
  result = GetNumDisplay(result,digital);
  tracefmt("%s+%s=%s", num1, num2, result);
  return result;
}
