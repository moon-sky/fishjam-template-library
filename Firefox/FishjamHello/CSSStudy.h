#ifndef CSS_STUDY_H
#define CSS_STUDY_H

#pragma once
/*******************************************************************************************
* TODO：
*   1.FF 有一个默认的 userChrome.css 文件?( 没有看到使用，但有一个 userChrome-example.css )
*   2.有三种CSS规范（一级到三级），其功能特性逐渐强大，Gecko渲染引擎处理的基本上都是二级CSS和一些三级CSS。
* 
* CSS(Cascading Style Sheets) -- 层叠样式表，解决内容与表现分离的问题，让两者可以独立变化，改变XML文档显示的样式描述语言。
*   CSS学习文档： http://www.w3school.com.cn/css/
*   CSS参考手册： http://www.w3school.com.cn/css/css_reference.asp
*
* 引用方式：
*   <?xml-stylesheet href="chrome://global/skin/global.css" type="text/css"?>  <== global.css 文件包含了所有XUL元素的缺省声明，主要是和当前主题匹配
* 
* 同一个HTML元素被多个样式定义时，所有的样式会根据规则层叠于一个虚拟样式表中，优先顺序为（由低到高）：
*   1.浏览器缺省设置(如 userChrome.css ? )
*   2.外部样式表 ( 在<head>处引入 .css 文件 )，如 <link rel="stylesheet" type="text/css" href="mystyle.css" />
*   3.内部样式表（在<head> 标签内部定义), 如：<head><style type="text/css">样式列表</style></head>
*   4.内联样式（在 HTML 元素内部），在相关的标签内使用样式(style)属性，如 <p style="color:red; margin:20px"> xxxx </p>
*     注意：尽量不要使用这种方式。
*******************************************************************************************/


/*******************************************************************************************
* class="xxx", 注意：类名的第一个字符不能使用数字
* id="xxxx"
* 属性名="属性值"  <== 可以自定义任何属性 
*
* 语法：选择器 { 声明1; 声明2; ... } <== 即： selector1, selector2 { 属性1:值1; 属性2:值2; ... }
*******************************************************************************************/

/*******************************************************************************************
* 选择器：通常是需要改变样式的 HTML元素。
*   1.选择器分组：用逗号将需要分组的选择器分开，各个选择器可以共享相同的声明。
*   2.派生选择器（或称为上下文选择器、后代选择器、包含选择器等,用空格分隔的选择器序列)：
*     如 li strong { xxx; } 表示控制 "列表(li)中的strong元素"
*     注意：两个元素之间的层次间隔可以是无限的，如上例中表示li中继承的所有strong元素，不论 strong 嵌套层次有多深
*   3.子元素选择器（使用大于号">"连接选择器序列) <== 只选择某个元素的子元素，不像派生选择器那样能递归嵌套。
*     父元素 > 子元素 { xxxx;}
*   4.相邻兄弟选择器(使用加号"+"连接选择器序列) <== 选择紧接在另一元素后的元素，且二者有相同父元素(注意：不影响第一个元素)
*     h1 + p {xxx} <== 选择紧接在 h1 元素后出现的段落，h1 和 p 元素拥有共同的父元素
*   7.id选择器("#元素id")：为标有特定 id 的 HTML 元素指定特定的样式，在现代布局中，id 选择器常常用于建立派生选择器。
*     如 #sidebar p { xxxx } <== 应用于出现在 id 是 sidebar 的元素内的段落
*   8.类选择器(".元素类名")：应用于属于特定类的HTML元素上，也可以建立派生选择器，如 ".myclass td" 等价于 "td.myclass" ?
*   9.属性选择器( [属性名] { xxx; } )：为拥有指定属性的 HTML 元素设置样式(说明：IE7及以上在规定了 !DOCTYPE 的情况下支持属性选择器)
*     如果采用 "*[属性名]" 的方式，表示应用于 包含特定属性的所有元素; 同理："a[href]"表 只对有href属性的锚(a元素)应用样式。
*     元素[属性名1][属性名2] <== 表示应用于 同时有属性1和属性2的<元素> 上， 可以通过增加属性值来选择具有特定属性值的元素
*       例：img[alt] {border: 5px solid red;} <== 突出显示有 alt 属性的图像，可以帮助确认图像是否确实有效？
*           测试后发现即使有图片，仍然会突出显示，尝试添加对DOM对象的 clientHeight 和 clientWidth 属性值的判断?
*           http://www.w3school.com.cn/tiy/t.asp?f=csse_selector_attribute_att_4
*     如果需要根据属性值中的词列表的某个词进行选择（部分匹配)，则需要使用波浪号（~)。
*       例：p[mytype~="important"]{xxx} 表示想选择mytype属性中包含 important 的元素
*     子串匹配属性选择器(CSS2的规则,IE7后支持)：
*       [abc^="def"]	<== 选择 abc 属性值以 "def" 开头的所有元素
*       [abc$="def"]	<== 选择 abc 属性值以 "def" 结尾的所有元素
*       [abc*="def"]	<== 选择 abc 属性值中包含子串 "def" 的所有元素
*     特定属性选择类型( |="xxx" )  <== 通常用于匹配语言值
*       *[lang|="en"] { xxx } <== 表示 选择 lang 属性等于 en 或以 en- 开头的所有元素
*
*  CSS伪类 (Pseudo-classes) <== 用于向某些选择器添加特殊效果，典型的是超链接的各种状态
*    语法： selector : pseudo-class { property: value; } 或 selector.class : pseudo-class {property: value;}
*    超链接(a) 状态的伪类，注意：各种链接状态伪类的顺序不能更改，否则不会生效
*      a:link		<== 未访问的链接
*      a:visited	<== 已访问的链接
*      a:hover		<== 鼠标移动到链接上
*      a:active		<== 选定的链接
*    拥有键盘输入焦点的元素(如输入框input)的伪类(有"!DOCTYPE"的时候 IE8 及其以后支持, FF/Chrome ?)
*      input:focus { xxx; } 
*    元素的DOM树伪类(有"!DOCTYPE"的时候 IE8 及其以后支持, FF/Chrome ?)，选择元素的第一个或最后一个子元素，如：
*      p:first-child <== 作为某元素第一个子元素的所有 p 元素，注意：★不是选择 p 元素的第一个子元素★
*      li:last-child <== 作为某个元素（在HTML中，这肯定是ol或ul元素）最后一个子元素的所有 li 元素
*    :lang伪类 <== 允许为不同的语言定义特殊的规则。
*      如 q:lang(no){ quotes: "~" "~" } 表示为带有值为 "no" 的lang属性的q元素定义引号的类型
*         使用时：<p>文字<q lang="no">段落中的引用的文字</q>文字</p>
*
*  CSS伪元素(Pseudo-elements) <== 用于向某些选择器设置特殊效果
*    语法：selector:pseudo-element {property:value;} 或 selector.class:pseudo-element {property:value;}
*    :first-line <== 向文本的首行设置特殊样式
*    :first-letter <== 向文本的首字母设置特殊样式
*    :before|after <== 可以在元素的内容前面(后面)插入新内容, 如 h1:before{content:url(logo.gif);} 表示在每个<h1>元素前面插入一幅图片
*******************************************************************************************/

/*******************************************************************************************
* TODO：
*   1.通过 DOM Inspector 中的 "Computed Style"可以查看元素能支持的所有属性
*   2.如果在属性值后面加上 "!important" 表明不能被覆盖 ? 一般来说，图片、文字等属性上会设置这个
* 
* 常见属性和可选值( )：
*   background-attachment : fixed <== 背景关联，默认是scroll(背景和文字一起滚动)。
*   background-color: yellow <== 背景色，默认是透明(transparent)（background 可以控制多个？顺序？）
*   background-image: url("chrome://xxxx/skin/back.png");  <== 背景图片
*   background-position : center ; <== 改变图像在背景中的位置, 如要表示右上角需要用 :top right;
*   background-repeat: repeat|repeat-x|no-repeat; <== 背景图像重复平铺，默认是 repeat
*   border: 1px dotted #000000; <== 一个像素宽的黑色点状边框
*   color:red|#ff0000;  <== 前景色，颜色有几种表示方式：
*     1.英文单词(如 red/black 等)；
*     2.以"#"开头的十六进制颜色值；
*     3.rgb(255,0,0);
*   crop <== 裁剪元素的内容，当元素的宽度超过maxwidth时，超出的部分会去掉，变成"...", 可用在 label(其他?) 元素中
*   display <== ?
*   flex <== 设置元素是否自动扩展，当box发生变化时，拥有flex属性的元素会分享变化的空间，按照flex之间的比率来分享。只对没有指定宽度的元素起作用。
*   font-size:14px; <== 设置字体大小
*   font-family:"sans-serif"; <== 字体系列
*   font-style: italic;	<== 字体格式，如斜体
*   font-weight: normal|bold;	<== 字体粗细
*   font-varian: small-caps <== 控制字体的大小?(会把英文字体全部转换为小型的大写字母)
*   line-height:1.5; <== ?
*   margin: 0;  <== 外边距? 对应的有 margin-top 等细化部分
*   ?orient: horizental | ?  <== 用在 window/box 等容器中，控制自动对齐方式
*   padding: 1px; <== 内边距
*   quotes: "前符号" "后符号";  <== 设置包含引用(<q>)文字的符号(系统默认是引号")，注意：需要设置两个
*   resize: none| <== 用在 window 等元素中，控制是否可以控制大小?
*   text-align: left|center|right|justify;  <== 文字水平对齐,justify表两端对齐， (垂直方向的是 vertical-align?)
*   text-decoration: none|underline|overline|line-through|blink;<== 文本装饰，如加下划线|上划线|删除线|闪烁等
*   text-indent : 2em; <== 段落的首行缩进
*   text-transform: uppercase|lowercase|capitalize; <== 文字的特殊转换 -- 有时不生效? font-varian: small-caps 能生效?
*   visibility : collapse  <== 可以控制收起(高和宽为0，但元素还是存在)
* 
* 单位(width/height/minwidth/minheight/maxwidth/maxheight)
*   http://kb.mozillazine.org/Em_units_versus_ex_units
*   px <== 像素?
*   em <== 文字的个数? 比如 text-indent:2em 表示首行缩进2个文字? 
*   ex <== 
* 
* 继承：通常元素从父元素继承属性（如 body 内部的元素属性会继承 body 的属性)，★但各个浏览器不一定按这个规则实现★
*    
* css 中控制Element: 
*  ".元素类名" { 元素Style }
*  @-moz-document domain("域名") { .元素类名 > div {display:none;} }   <== 什么意思 ?
*******************************************************************************************/

#endif //CSS_STUDY_H