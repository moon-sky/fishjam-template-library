#ifndef FF_XUL_H
#define FF_XUL_H
#pragma once

//TODO: 各种基于XML的语言（如XHTML和SVG）中的元素都可以嵌入到另外的元素中作为一种扩展语言的方法。
//      此时就要通过名称空间进行区分，如 XHTML("http://www.w3.org/1999/xhtml") 和 SVG("http://www.w3.org/1999/svg" ?)
//      XUL 的名称空间为？： http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul
//
//可以通过 XUL Gear 自动生成 相关的XUL文件?
/*******************************************************************************************************
* 注意：
*   1.帮助中用代码填充<listbox>数据的方式。
*   2.根据不同语言来设置UI的参数(如长宽) -- 如：style="width:&attachmentextractor.settings.width;; height: 55em;"
*     然后在 dtd 中根据不同的语言设置对应的宽度。注意 上面的 "width" 后有两个分号,分别是 资源结束 和 style项间隔。
*     http://www.babelzilla.org/index.php?option=com_content&task=view&id=357
*   3.现在如何支持"远程XUL"技术 -- 即通过 file:///C:/abc.xul 等的方式显示外部xul。 security.checkloaduri ？
*    
* 学习地址：https://developer.mozilla.org/en-US/docs/XUL  / https://developer.mozilla.org/en-US/docs/XUL_Reference
*           http://wiki.woodpecker.org.cn/moin.cgi/LearningXul
*           http://blog.donews.com/limodou/archive/category/xul
* 
* XUL(XML User Interface Language) -- 应用XML来描述用户界面的标示语言，完成UI定制，既可以在远程执行，也可以安装到本地执行。
*   通过 Gecko 描绘引擎显示。通过"XUL Periodic Table"工具可以查询各种各样的布局?
*   XUL提供了一套简易和跨平台的widget定义,可以创建现在图形界面绝大多数的元素, 同时描述了事件处理、风格(style) 和字符串国际化等信息，可以直接被mozilla layout引擎解析执行。
*   XUL Explore -- 轻量级的XUL集成开发环境(https://developer.mozilla.org/en-US/docs/XUL_Explorer)，但是测试好像不好用
*   XUL文件中也可以写XHTML代码(但不建议)，方法是引入XHTML名字空间（在window节点增加属性 xmlns:html="http://www.w3.org/1999/xhtml"）,
*   且所有的XHTML标签都要加 html:,比如<p>要写成 <html:p>
* 
* 一个XUL界面一般包含以下三个部份：
*   内容: 定义使用者界面的XUL档
*   Skin: 定义程序外观的CSS和图像档
*   Locale: 定义界面字串的DTD档（为了软件本地化）
*   
* Overlay -- 可以将多个XUL 文档联合起来，并当做一个单独的XUL文档来处理。FF中用来对各种功能进行模块化并实现了扩展。
*   可设置<xul-overlay>处理指令(类似xml-stylesheet?)来主动要求合并其他的XUL文件?
*   如 <?xul-overlay href="overlayDocument.xul"?>， 对应的"overlayDocument.xul"文档需要用 <overlay>做为根元素
*   
* 
* 工具
*   xulrunner.exe -- 
* 
* XUL(https://developer.mozilla.org/En/XUL)
*   在线的XUL编辑器(不过实测发现不再可用？) -- http://ted.mielczarek.org/code/mozilla/xuledit/xuledit.xul
*                                              http://www.hevanet.com/acorbin/xul/top.xul
*
* XUL 可以内嵌JS脚本(但最好还是写在独立的外部文件中)，需要将代码括在 CDATA 段中
*   (可以避免由读取&加其他字符开头的实体引用时出现错误)，如：
*   <script type="application/javascript"><![CDATA[
*     var nodes = gBrowser.mTabContainer.childNodes;
*     for (var i = 0; i < nodes.length; i++)
*     alert(nodes[i].label);
*   ]]></script>
*******************************************************************************************************/

/*******************************************************************************************************
* 常见的通用属性
*   [align/pack] -- 指定元素布局，属性缺省为stretch(扩展到最大以匹配其所在的元素)，其他有 start(顶端|左端对齐), 
*      center(居中), end, baseline(水平线对齐，只能用在hbox上)。
*      align属性定义为与orient属性相垂直的方向，而pack属性与orient属性的操作方向一致。组合起来即可在9方位上进行布局。
*   [class] -- 为元素分类
*   [collapsed] -- 如果设置为"true" 表示将一个元素的高和宽设置为0， 但是这个元素还是会被认为是存在的，与 CSS 中设置"visibility:collapse"效果相同。
*   [crop] -- 文本宽度大于其父元素的宽度时被省略号(…)代替
*   [disabled] -- 禁用对象，通常显示成浅灰色或者透明的
*   [flex] -- 说明了一个元素应该扩展以占据一个窗口的高度和宽度的比例，值为正整数，表示在父元素的orient属性方向上的增长倍数。
*   [height/width] -- 设置准确的尺寸
*   [hidden] -- 控制元素是否显示，和 CSS 中设置 "display: none" 效果相同。
*   [id] -- 定义一个元素的唯一标识名称的
*   [minwidth/minheight/maxwidth/maxheight] -- 通常在设置了flex属性后表示大小可以变化后，设置最小和最大尺寸。
*      TODO:CSS中是 min-width ?
*   [ordinal] -- 设置布局顺序(默认按照代码中出现的顺序布局),
*   [orient] -- 竖直(vertical)或水平(horizontal)排列
*   [persist] -- 持久化数据("Persistent Data" )，需要保存元素的一些状态数据(如窗口的位置和大小)时，设置 persist 属性为空格分开的属性名即可，
*      如：针对 window 的 persist="screenX screenY width height"。  
*      注意：1.要保存数据的对象必须有 "id" 属性;
*            2.对应的值保存在用户配置目录下的 localstore.rdf 文件中
*   [tooltiptext] -- 工具提示，只能设置提示文本，如果想更丰富的内容，需要使用 tooltip 元素。
*   
*
* 常见Widget
*   <arrowscrollbox> -- 带箭头的滚动框，其内部可以放置任何元素(系统会自动使用？)
*   <box> -- orient 属性可选 horizental(水平，等价于 hbox) 或  vertical(垂直, 等价于 vbox)
*   <browser> -- 功能比<iframe>更强大的内联帧，具有网页浏览器的所有基本功能(前进/后退/阻止内部脚本访问外部资源)
*     [type] <== 如设置为"content"会设置访问限制(所有打开的网页都将其视为最顶层的帧，不能访问父帧?)
*                如设置为"content-primary"，表示与其他XUL文档不同的特殊浏览器，窗口对象可使用"window.content"来访问
*   <button> -- 按钮，可以通过 type="menu" 且内嵌 <menupopup> 来实现具有下拉菜单的按钮，如果type设置为 menu-button ?
*      通过[icon]或[image]属性显示图像?
*   <caption> -- ?
*   <checkbox> -- 复选框，选中时 [checked]为true
*   <command> -- 统一处理来自多个源(如菜单项、快捷键等 )的命令，需要设置 id属性，然后设置源的 command 属性。
*                如果包含了 "chrome://global/content/globalOverlay.js"后即可通过 goDoCommand 调用命令
*     [oncommand] -- 事件处理程序
*   <commandset> -- 不可见元素，内嵌 <command>，通常可在 对 <window id="main-window"> 的 overlay 中设置一些全局可调用的 command
*   <conditions> -- ? rule 中用来进行匹配使用的条件，它有三个子元素：content, member, triple。
*      <content> -- 只能写一次。用来将包含模板的元素中的rel所指的起始根对应到一个变量中
*      <member>  -- 用来映射每条资源的变量
*      <triple>  -- 三元组，用来定义匹配规则,一个triple由三要素组成：主题(subject), 谓词(predicate), object(目标)。
*   <context> -- 上下文菜单，需要放在 <popupset> 里且其他元素通过 [context] = contextId 进行引用
*   <dialog> -- 对话框窗口的根元素
*      [buttons] <== 用逗号分隔的设置在其底部显示的按钮( accept,cancel,help,disclosure--其他信息)，会自动根据OS进行布局(如Mac OS 将Cancel放在左边?)
*                    两个特殊按钮, extra1 使用标签 buttonlabelextra1， extra2 使用标签 buttonlabelextra2。
*                    处理事件: ondialog<buttonname>，需要 "XPConnect privileges" ,
*   <deck> -- 可容纳多个子元素，但一次只有一个子元素是可见的。通过 selectedindex 属性来切换可见的子元素，第一个子元素的索引为0。
*   <description> -- 长段的描述文档
*   <frame> -- 内嵌帧?
*   <grid> -- 表格布局，其内嵌 <columns><column>(记录每条记录的字段信息，一般不放内容，主要用来控制外观,如 [flex]) 
*             和 <rows><row>(一般用于放置内容) 控件来显示
*             使用grid比嵌套的box的最大好处是可以让创建的单元格在水平和垂直方向同时扩展时要方便得多
*   <groupbox> -- 组框，可以摆放多个元素，显示一个框和标题
*   <iframe> -- 显示其他的页面( 用的很少，实际上常用<browser> ?)
*     [src] <== 指定其他页面的URL地址(如 http://xxx, chrome://xxx)
*   <image> -- 通过 src="image.jpg" 使用图片(更好的方式是使用CSS?)
*     list-style-image <== 提会自动旋转做动画?(如 url("chrome://browser/skin/tabbrowser/loading.png") )
*     list-style-type: square <== 图像不可用时?
*   <key> -- 定义 window 全局的快捷键，可以设置<menu>或<menuitem>的 [key]为本元素的id
*     [command] <== 设置 command 元素的id，这样可以多个源统一处理； 也可以设置 [oncommand] 属性
*     [modifiers] <== 设置功能键，如 "accel,alt", 其中 accel 表示根据系统来确认功能键(Win下是Ctrl,Mac上是command)
*     [key] <== 对应的按键，如 "C",注意：不区分大小写
*     [keycode] <== 特殊按键的虚拟键，如 "VK_UP","VK_DELETE" 等
*   <keyset> -- 不可见元素，内嵌 <key>对象，设置快捷键
*   <label> --  独立的文本标签，[value]="显示字符"，如要显示长文本，需要采用设置[flex]="1"且内嵌文本的方式(不设置[value]属性)
*     [control] ="myControlId" <== 指定关联的控件，点击label会使控件的输入焦点激活
*   <listbox> -- 列表框，通过 <listhead><listheader label="XXX">(定义表头) 和 <listcols><listcol>(定义列的外观) 设置外观，通过 <listitem> 设置内容，
*      其内部可以包含任何元素。
*   <menu> -- 系统菜单栏中的一项，如 "File",内嵌 <menupopup> 提供下拉菜单， 也可以作为二级菜单的 父元素(即替换对应的 <menuitem>,然后内嵌 <menupopup>
*   <menubar> -- 系统菜单栏，内嵌 <menu><menupopup><menuitem> 提供系统菜单功能
*   <menuitem> -- 弹出菜单中的菜单项，其父元素通常是 <menupopup>，通过 label 提供显示的文本
*      如果 class="menuitem-iconic", 则可以通过 image 属性设置菜单图标
*      [type] <== checkbox(选中标记，选中项的"checked"=true),radio(多个name属性相同的为一组，选中项的selected="true")
*   <menulist> -- 下拉列表框，其内嵌 menupopup 。
*     [editable] <== 如设置为"true"，则也可以不从下拉列表中选择，而手工输入(如文本选择器)
*   <menupopup> -- 弹出菜单，不可见(即不需要设置 label 属性)，内嵌 <menuitem> 和 <menuseparator> 等
*   <menuseparator> -- 菜单的分割条
*   <overlay> -- 可以合并到其他XUL中去的XUL文件的根元素？
*   <page> -- 侧边栏以及其他在窗口框架内部打开的XUL文档等的根元素，需要内嵌 iframe 指向 XUL 文件 来提供内容
*   <popupset> -- 弹出内容，其内部包含 <menupopup>,<panel>,<tooltip> 三种元素，分别对应 上下文菜单([context]),普通弹出(左键点击),工具提示([tooltip])。
*      注意：在设置的位置不可见，需要通过其id关联弹出方式，当条件满足时才显示出来。
*   <prefwindow> -- 设置窗体? 具有确定和取消的按钮
*   <progressmeter> -- 进度条，通过 mode="determined(确定的)|undetermined(不确定的)" 选择类型;
*      当模式为确定时，可以通过 value="50%" 设置进度位置; 当为不确定的时，进度持续更新表示处理正在继续
*   <radio> -- 单选按钮，当前选择的元素[selected]="true",且所在<radiogroup>的[value]属性为选择<radio>对应的值
*   <radiogroup> -- 内嵌 radio 按钮，只有一个能选中
*   <rule> -- 在处理模板生成的数据时指定规则？可以内嵌 <conditions>,<bindings>,<action>三个子元素。
*      模板生成器会先根据conditions中的条件对资源进行匹配，如果匹配成功，则调用相应的action中的内容进行复制处理
*   <scale> -- 滑动条(类似音量控制滑块)
*   <scrollbar> -- 滚动条，可通过 orient 设置方向，
*      [slider?]
*   <spacer> -- 占位空白。如果flex属性为"1"且当前父元素中只有一个元素有该属性时则当窗口变化时自动变化，通常可放在需要右对齐的按钮前。
*   <splitter> -- 分割条，通过拖动可以用来改变并列元素的大小。
*      [state] <== 在初始显示时，collapse所指明的元素是打开还是关闭。当state值为"open"是为打开。state为"collapsed"时为关闭。
*      [collapse] <== 指明哪个元素可以被关闭(隐藏)，可选值是 before|after|both|none 
*   <stack> -- 栈容器，其内部的元素是按创建的顺序重叠摆放，新创建的放在最上面，
*      用途：把颜色不同的字错开重叠可以实现一些阴影效果；<progressmeter>和<label>一起显示带文本的进度条
*   <stringbundle> -- 通过 src="chrome://xxx/locale/xxxx.properties" 的方式指定属性文件，属性文件是一串 Key=Value 对的文本文件。
*      然后 JS 中即可通过如下方法获得响应的本地字符资源：
*        var bundleString = document.getElementById("bundle的ID"); var strValue = bundleString.getString("想要的资源Key");
*   <stringbundleset> -- 内嵌 <stringbundle> 元素，在JavaScript中提供本地化
*   <tabbox> -- 标签页, 需要组合 <tabs><tab label="标签页的标题"/>  和 <tabpanels><tabpanel>标签页的内容</tabpanel> 才能显示出来，
*      两者的个数和顺序需要对应。TODO: 有没有办法在 tabpanel 中引用到别的 xul 文件？
*      <tab> 使用 selected="true" 来选定
*   <tabbrowser> -- 比<browser>更强大，包含了标签处理特性的基本内容，需要 XPConnect 权限
*   <textbox> -- 文本输入框
*     [multiline] <== 为"true"时可以输入多行文本
*     [type] <== 为"password"时可以输入口令, 为"search"时会对搜索功能扩展，
*        为"number"时输入数字且有数字微调按钮，通过[min][max][increment]设置更详细的信息
*        为"autocomplete" 则支持自动完成，还需要设置[autocompletesearch]属性指定搜索目标，如"history"表地址栏历史,
*        "form-history"表其他表格的输入历史(然后怎么做?)。注意：需要 XPConnect 权限
*     [oninput] <== 处理用户输入的实时事件
*   <toolbar id="xxx"> -- 工具条，内部组合 <toolbarbutton> 等
*      [defaultset] -- 指定用逗号分隔开的多个 <toolbarpalette>
*      [grippyhidden] -- 设置为 true 表示关闭工具条的小槽(toolbargrippy)
*   <toolbarbutton> -- ?定义工具栏按钮的元素
*      [type] <== 可选"checkbox","menu","menu-button(如后退/前进按钮)" 等
*   <toolbargrippy> -- 
*   <toolbaritem> -- 可以自嵌套
*   <toolbarpalette> -- 用在<toolbar>中的一个面板，可以内嵌 <toolbarbutton> 等按钮。本身不可见，但被<toolbar>用来定制items的显示。
*      可在 overlay 中通过设置 id 为 BrowserToolbarPalette 来更改Firefox的工具条
*   <toolbox> -- 对工具条编组，需要组合 <toolbar><toolbarbutton> 等使用，其左侧有一个小槽，点击可打开或关闭工具条
*   <tooltip> -- 定义工具提示，其内部可以包含任何元素，设置好后通过设置需要显示提示的元素的 tooltip="myTooltipID" 来关联
*   <tree> -- 可以生成分级结构和分级的列表结构，但只能包括文本或图片，可以使用数据源，利用模板来生成。
*   <window> -- 顶级普通窗口，如 Firefox浏览器窗口、书签管理器窗口 等
*     [windowtype] -- 窗口类型标识符，可以利用这个属性把相同类型的窗体组合在一起。如 浏览器窗口"navigator:browser", 选项窗口"Browser:Preference" 等
*   <wizard> -- 显示向导页
* 
*   
*   
* 模板生成数据(Mozilla中的方式，不知道是否适合Firefox?)
*   1.设置元素的 [datasources] 和 [ref] 属性指定数据源
*   2.提供一个元素的模板(template)表示一个搜索条件，可以把目标XML文件中的满足条件的元素找到 -- 类似 xslt ?
*     在模板中的元素属性值如果是以rdf:开头的，表明这个值应该是从数据源来的。rdf:后面的值表示数据源中的属性名。
*   3.可以使用 rule 元素来对一些资源进行不同的处理
*   例子：将bookmarks 中的书签生成按钮(是否能从Firefox中找到源码?)
*
*   怎么生成自己的RDF数据源 ?
*
*
* 事件处理
*   XUL的事件处理方式与HTML中的基本类似。对于需要响应事件的元素，只要在元素中增加相应的事件处理函数即可。
*   XUL使用与DOM2 Events描述的事件模型一样
*     一个事件的发送有两个状态：捕获状态和冒泡状态。
*     在捕获状态，一个事件先发送到document，然后按照元素的分级结构层层传递，直到触发事件的元素。
*     冒泡状态是反方向传递。
*     在处理路径上，如果事件被某个元素处理，那么事件传递即结束，如果没有被处理，则继续传到下一个元素。
*     如果都没有处理，则缺省的处理被执行。这就意味着，不一定要把事件处理放在触发事件的元素上，而是放在它的父元素中进行处理。
*     如果一个事件被处理，处理函数应该返回true，如果返回false则表示未被处理。因此，有时可以在处理了事件后，仍然返回false，从而使事件可以继续传递。
*     可以使用event对象的target属性来得到触发事件的元素，如 onclick="dump(event.target.tagName + '\n'); return false;"
*******************************************************************************************************/



#endif //FF_XUL_H