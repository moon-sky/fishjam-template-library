#pragma once

/******************************************************************************************************
* DOM(Document Object Mode) -- 文档对象模型，可以以独立于平台和语言的方式访问和修改一个文档的内容和结构，
*   以对象的方式处理XML文档内容。Javascript通过文档对象模型(DOM)来操作XUL中的元素。
*
* 
* tabbrowser -- 浏览器核心?
* 
* 各种页面事件
*   autocompletepopup -- 
*   onnewtab --
*
* 属性
*   document -- 全局变量 ?
*     write()
* 
* 页面事件：
*   window.addEventListener("load", xxx);
*   appcontent.addEventListener("DOMContentLoaded", this.onPageLoad, true); //var appcontent = document.getElementById("appcontent");
*   window.addEventListener("pagehide", OnPageUnload, false);
* 
* 由于gecko引擎实现了2 级DOM的事件处理器，因此可以定义动态的事件侦听器。
*   var item = document.getElementById('menu-item-custom');
*   function handleCommandEvent(aEvent) {
*     alert('OK');
*     item.removeEventListener('command', handleCommandEvent, false);
*     item.parentNode.removeChild(item);
*   }
*   item.addEventListener('command', handleCommandEvent, false);
* 
******************************************************************************************************/
#if 0
//Sample 
var bar = document.getElementById('toolbar');
bar.removeChild(bar.childNodes[1]);
bar.appendChild(document.createElement('button'));
bar.lastChild.setAttribute('label', 'Hello!');

#endif
