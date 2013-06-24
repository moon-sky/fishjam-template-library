/*************************************************************************************************************************
* 三种后台执行方式的封装(统一接口，根据运行环境选择实现) -- https://code.google.com/p/jsworker/
*
* 后台执行：浏览器中 JavaScript引擎 是单线程执行的，一般来说实现方式是依次从内部待处理的事件队列中取出事件来执行。
*   timer = setTimout(func, time [,args]) -- 设置在指定的间隔时间之后执行某个 JavaScript 方法，清除时使用 clearTimeout(timer)
*     特殊用法：把一个需要较长执行时间的任务分成若干个小任务，这些小任务之间用 setTimeout()串联起来
*   id = setInterval(func, time [,args]) -- 设置根据指定的间隔重复执行某个 JavaScript 方法，清除时使用 clearInterval(id)
* 
*   Google Gears -- Google开发的浏览器插件工具(已被Web Worker代替)，可以增强现有浏览器的功能(应用程序本地缓存、本地数据库和 JavaScript 工作线程池等)
*     google.gears.factory.create('beta.workerpool') -- 创建一个工作线程池，通过消息传递机制进行交互(onmessage)
*     createWorker(scriptText) -- 创建执行脚本的工作线程任务，返回标识符
*     createWorkerFromUrl(scriptUrl) -- 创建根据URL进行下载并执行的工作线程任务，返回标识符
*     sendMessage(message, destWorkerId) --  线程之间进行信息交互
*   
*   Web Worker -- HTML5 中借鉴Goolge Gears，形成的使用工作线程的标准方法
*     专属工作线程(Worker) -- 只能为一个页面所使用，通过指定了要执行的JavaScript文件的URL的 Worker 构造方法创建
*     共享工作线程(SharedWorker) -- 可以被多个页面所共享使用，每个页面都是连接到该共享工作线程的某个端口（port）上面。页面通过此端口与共享工作线程进行通信。
*       
*************************************************************************************************************************/

module("ThreadTester", {
	setup: function() {
		//dump("in MathTester::setup\n");
	},
	teardown: function() {
		//dump("in MathTester::teardown\n");
	}
});

test("setTimeoutTest", function() {
    ok(1 == 1, "TODO:setTimeOut");
});

//google Gears 插件中提供的JavaScript线程池功能
test("googleGears", function() {
/*
	var workerPool = google.gears.factory.create('beta.workerpool');

	workerPool.onmessage = function(a, b, message) {
		var result = message.body;
		alert("计算完成，质数个数为：" + result);
	};

	function calculate() {
		var limit = parseInt(document.getElementById("limit").value) || 100000;
		var childWorkerId = workerPool.createWorkerFromUrl('prime.js');
		workerPool.sendMessage(limit, childWorkerId);
	}
	
	
	//prime.js 文件中 工作线程所执行的 JavaScript 代码中的通信部分
	var wp = google.gears.workerPool; 
	wp.onmessage = function(a, b, message) { 
		var limit = message.body; 
		var count = calculateNormal(limit); 
		wp.sendMessage(count, message.sender);  //通过 message.sender可以获取到发送消息的工作线程池的标识符，并把计算结果通过 sendMessage()发送回去
	}
*/
	ok(1 == 1, "TODO: Google Gears");
});

test("WebWorkerTester", function() {
    ok(1 == 1, "TODO: WebWorkerTester");

    //主页面创建了一个专属工作线程，让它在后台执行 JavaScript 文件 prime_worker.js
	/*
	var worker = new Worker("prime_worker.js"); 

	 worker.onmessage = function(event) { 
		var result = event.data; 
		alert("计算完成，质数个数为：" + result); 
	 }; 

	 function calculate() { 
		var limit = parseInt(document.getElementById("limit").value) || 100000; 
		worker.postMessage(limit); 
	 }
    */
 
	/*
	//专属工作线程所执行的 JavaScript 代码 -- 用 onmessage来直接定义接收到消息时的处理方法
	onmessage = function(event) { 
		var limit = event.data; 
		var count = calculateNormal(limit); 
		postMessage(count); 
	}
    */
});

test("WebSharedWorkerTester", function() {
    ok(1 == 1, "TODO: WebSharedWorkerTester");

    /*
	var worker = new SharedWorker("prime_worker.js"); 

	worker.port.onmessage = function(event) { 
		var result = event.data; 
		alert("计算完成，质数个数为：" + result); 
	}; 

	function calculate() { 
		var limit = parseInt(document.getElementById("limit").value) || 100000; 
		worker.port.postMessage(limit); 
	}
	*/
	
	/*
	//共享工作线程所执行的 JavaScript 代码 -- 需要使用 onconnect来定义接收到连接时的处理逻辑
	onconnect = function(event) { 
		var port = event.ports[0]; 
		port.onmessage = function(event) { 
			var limit = event.data; 
			var count = calculateNormal(limit); 
			port.postMessage(count); 
		}; 
	}
     */
});