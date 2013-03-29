//检查是否支持HTML5的视频功能
function checkHtml5Video() {
	if (!!document.createElement('video').canPlayType) {
		var vidTest = document.createElement("video");
		oggTest = vidTest.canPlayType('video/ogg; codecs="theora, vorbis"');
		if (!oggTest) {
			h264Test = vidTest.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"');
			if (!h264Test) {
				document.getElementById("checkVideoResult").innerHTML = "Sorry. No video support."
			}
			else {
				if (h264Test == "probably") {
					document.getElementById("checkVideoResult").innerHTML = "Yes! Full support!";
				}
				else {
					document.getElementById("checkVideoResult").innerHTML = "Well. Some support.";
				}
			}
		}
		else {
			if (oggTest == "probably") {
				document.getElementById("checkVideoResult").innerHTML = "Yes! Full support!";
			}
			else {
				document.getElementById("checkVideoResult").innerHTML = "Well. Some support.";
			}
		}
	}
	else {
		document.getElementById("checkVideoResult").innerHTML = "Sorry. No video support."
	}
}

//判断是否为质数
function isPrime(n) {
	if (n == 0 || n == 1) {
		return false;
	}
	var bound = Math.floor(Math.sqrt(n));
	for (var i = 2; i <= bound; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

//直接计算小于等于 maxNumber 的所有质数的个数 -- 可能耗费很长的时间
function calculateNormal(maxNumber) 
{   
	var count = 0;
	for (var i = 2; i <= maxNumber; i++) { 
		if (isPrime(i)) {      
			count++;     
		}   
	}
	return count;
}

//通过 setTimeout()把计算任务分成若干个小任务的方式来计算小于等于 maxNumber 的所有质数的个数
//TODO: 怎么返回?
function calculateUsingTimeout(maxNumber) {
	var jobs = 10, numberPerJob = Math.ceil(maxNumber / jobs);
	var count = 0;

	function calculate(start, end) {
		for (var i = start; i <= end; i++) {
			if (isPrime(i)) {
				count++;
			}
		}
	}

	var start, end, timeout, finished = 0;
	function manage() {
		if (finished == jobs) {
			window.clearTimeout(timeout);
			alert("计算完成，质数个数为：" + count);
		}
		else {
			start = finished * numberPerJob + 1,
                end = Math.min((finished + 1) * numberPerJob, MAX);
			timeout = window.setTimeout(function() {
				calculate(start, end);
				finished++;
				manage();
			}, 100);
		}
	}
	manage();
}

/*///////////////////////////////////////////////////////////////////////////////
//一个自动检测Form中输入元素(input)数据是否有效的方法
//onchange 在区域时区焦点时触发， oninput 在每次字符改变后触发
window.onload = function() {
	var inputs = document.getElementsByTagName("input");
	for (var i = 0; i < inputs.length; i++) {
		if (!inputs[i].willValidate) {
			continue;
		}
		inputs[i].onchange = function() {  //给 onchange 事件绑定匿名函数，调用HTML5提供的 checkValidity 进行数据有效性检查
			if (!this.checkValidity()) {
				this.style.border = 'solid 2px red';
				this.style.background = '';
			} else {
				this.style.border = '';
				this.style.background = 'lightgreen';
			}
		}
	}
}
//*/

///////////////////////////////////////////////////////////////////////////////