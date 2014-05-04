#ifndef _STL_STUDY_H
#define _STL_STUDY_H

#pragma once

/************************************************************************
* 标准模板库(STL) -- Standard Template Library,被容纳于C++标准程序库（C++ Standard Library）中,是其中的一个子集。
*   从逻辑层次来看，STL 体现了 泛型化程序设计的思想（generic programming -- GP）
*   从实现层次看，STL 是以一种类型参数化（type parameterized）的方式(模板template)实现的
*
* 
* STL的重要特点：
*   1.数据结构和算法的分离
*   2.不是面向对象的,为了具有足够通用性，STL主要依赖于模板而不是封装，继承和虚函数（多态性）——OOP的三个要素
*   3.基于模板，内联函数的使用使得生成的代码短小高效
* 
* 注意：由于MS实现的STL使用单独的临界区保护整个内存管理系统（C运行库），当在大规模的多线程应用程序中，
*   如果使用STL的默认内存管理，可能成为性能瓶颈。
*   更改措施：1.使用 HeapCreate 替代 malloc/new 为每个线程创建独立的堆；
*             2.使用 STLPort 等替代品；
*             3.使用多处理器的内存管理代码Hoard(http://www.hoard.com/)
*
* C++标准函数库 -- 为C++程序员们提供了一个可扩展的基础性框架
*   包含
*     1.语言支持（language support）部分 -- 包含了一些标准类型的定义以及其他特性的定义
*     2.诊断（diagnostics）部分 -- 提供了用于程序诊断和报错的功能，包含了异常处理（exception handling），
*       断言（assertions），错误代码（error number codes）三种方式
*     3.通用工具（general utilities）部分 -- 如 动态内存管理工具，日期/时间处理工具 等。
*     4.字符串（string）部分 -- 代表和处理文本
*     5.国际化（internationalization）部分 -- 采用locale和facet可以为程序提供众多国际化支持，
*       包括对各种字符集的支持，日期和时间的表示，数值和货币的处理等等
*     6.容器（containers）部分 -- STL的重要组成部分。涵盖了许多数据结构（如 list,vector,queue,stack 等）
*         分为顺序容器(Sequence Container) 和 关联容器(Associative Container) 两类
*     7.算法（algorithms）部分 -- 包含了大约70个通用算法，用于操控各种容器，算法已经保证了效率。
*     8.迭代器（iterators）部分 -- 指针的泛化，结合容器和算法。算法通过迭代器来定位和操控容器中的元素。
*         分为随即迭代器、双向迭代器、前向迭代器、输入迭代器、输出迭代器 等。
*     9.数值（numerics）部分 -- 包含了一些数学运算功能，提供了复数运算的支持。
*     10.输入/输出（input/output）部分 -- 经过模板化了的原有标准库中的iostream部分，它提供了对C++程序输入输出的基本支持，
*        并且支持国际化(internationalization)
*   
* 从根本上说，STL是 容器、算法、迭代器 和其他一些组件(配置器、适配器、仿函数) 的集合 -- STL通用算法都是可以操作不止一个容器的
* 
* STL彻底封装复杂的数据结构，提供丰富灵活的程序接口，同时还能保证程序的运行效率。
* STL 分类：
*   1.SGI : http://www.sgi.com/tech/stl/stl.tar  -- 目前应用最为广泛的STL版本,提供 SGI STL 源码下载，用于 GCC(Linux)，
*     注意：SGI的散列容器（hash_set 等）确定在一个散列容器中的两个对象是否有相同的值是通过相等测试(equal_to)，而不是等价(less)
*   2.STL Port : http://www.stlport.org/  -- 在 C++ Builder中使用.效率高，更符合标准，也更容易移植。线程安全的。
*       STLport提供“调试模式”来帮助侦测STL的不正确用法 -- 可以编译但导致未定义的运行期行为。
*   3.Dinkumware : http://www.dinkumware.com/ -- MS 使用的版本，非线程安全的
*   4.HP STL -- 其它STL实现版本的根源, 由STL之父创建，现在已经很少使用。
*
* SUN中的STL：
*   RW 的stl路径在 /opt/SUNWSpro/prod/include/CC/CStd 下
*   stlport 的在   /opt/SUNWSpro/prod/include/CC/stlport4 下
* 
* STLFilt -- STL出错消息澄清器（decryptor），通过把错误消息缩短来帮助理解.
*
* STL 的组成
*   1.容器 -- 如果容器是保存的对象，加入数据时会调用拷贝构造(可能还有赋值操作符)，在容器析构的时候会自动清理这些对象
*             如果它保存的是对象的指针，创建和删除指针是程序员的责任
*     根据内存分配方式进行分类：
*       a.连续内存容器(基于数组)，提供随机访问迭代器--在一个或多个（动态分配）的内存块中保存它们的元素。
*         如果一个新元素被插入或者已存元素被删除，其他在同一个内存块的元素就必须向上或者向下移动
*         来为新元素提供空间或者填充原来被删除的元素所占的空间。这种移动影响了效率和异常安全。
*       b.基于节点的容器，提供双向迭代器--在每个内存块（动态分配）中只保存一个元素。容器元素的插入或删除只影响指向节点的指针，
*         而不是节点自己的内容。所以当有东西插入或删除时，元素值不需要移动。
* 
* 向 STL 容器插入对象时，保存的是其拷贝构造，使用时也使用拷贝构造（拷进去，拷出来） -- 采用对象方式保存时，子类的信息在拷贝时会丢失。
*   ★一个使拷贝更高效、正确而且对分割问题免疫的简单的方式是建立指针的容器而不是对象的容器★--智能指针的容器 -- 使用 Boost库中的 shared_ptr
*
*     标准STL容器：
*       序列容器(vector/list/deque -- 线性时间查找),关联容器(set/multiset/map/multimap -- 对数时间查找)
*     非标准STL容器(散列容器)：
*       hash_set、hash_multiset、hash_map和hash_multimap -- 最好时常数时间查找
*     标准非STL容器：
*       数组、bitset、valarray、stack、queue和priority_queue
*     注意：
*       1.从查找效率来说，基于散列的容器 > 有序的vector > set/map
*       2.vector 具有最低的空间开销，存取速度最快，容器中相邻的对象在内存中也是相邻的，具有与C语言兼容的内存布局，通常可用于替换数组。
*         可用 &*v.begin(),&v[0],&v.front() 等方法来获取第一个元素的指针
*

*   2.迭代器：iterator 就象是容器中指向对象的指针,用来将算法和容器联系起来，通过迭代器能在尽可能少地知道某种数据结构的情况下
*     完成对这一结构的运算。
*     不同的迭代器有不同的作用：只读，只写，仅能向前指，双向的(list中)，随机存取(RandomAccessIterator) 等
*     注意：
*       list 不能用 begin()+2 来指向list中的第三个对象，因为STL的list是以双链的list来实现的， 它不支持随机存取--编译错误（C3767）
*       而 vector和deque(向量和双端队列)和一些其他的STL的容器可以支持随机存取 -- 可以编译通过
*       一个范围的最后一个位置实际上并不被处理(如 end() 方法得到的位置)
*       迭代器也可以是那些定义了operator*()以及其他类似于指针的操作符地方法的类对象,能够使用*操作符类获取数据
* 
*   3.算法 -- 使用iterator在容器上进行操作，通常是通用的。但有时容器支持它自己对一个特殊算法的实现，这通常是为了提高性能。
*       由于各种原因，容器在性能需要较高或有特殊效果需求的场合支持外部函数(extra functions)， 这通过利用构造函数的结构特性可以作到
* 
*   4.函数对象(function object，仿函数类的实例) -- 带有一个operator()方法的类，有些STL算法作为参数接收函数对象并调用这个函数对象的operator()方法
*       函数对象被约定为STL算法调用operator时返回true或false。它们根据这个来判定这个函数。
*
* TODO:
*   分配器(allocator,封装内存模式信息的抽象模块,使STL的大部分实现都可以独立于具体的内存模式，从而独立于具体平台)
*   容器的嵌套
*   字符串：ANSI/ISO
*   模板函数的偏特化（template function partial specialization），它被用于在特定应用场合，
*     为一般模板函数提供一系列特殊化版本。使得STL中的一些算法在处理特殊情形时可以选择非一般化的方式，
*     从而保证了执行的效率。
*   适配器adaptor
*   分配器：STL采用自己的Allocator分配内存，以内存池的方式来管理这些内存，会大大减少内存碎片，从而会提升系统的整体性能
*   注意：STL 中展开时可能会把 back_insert_iterator<vector<int>> 一类语句中右边的两个 > 符号认为是移位符号，而产生编译错误。
*     最好加上空格。比如       back_insert_iterator< vector<int> >
*   STL用于商业程序时的一个用法：一个程序中的函数对象是处于一个继承树中，顶层的函数对象调用低层的函数对象。
*     通过小心使用函数对象并给它们 起有意义的名字，我使它们在我的软件的控制流中流动。 
*   最好通过 typedef 来屏蔽使用的容器类型 -- 方便更改
*      typedef vector<Widget> WidgetContainer;  可以很容易的进行替换，如：
*        typedef vector<Widget, SpecialAllocator<Widget> > WidgetContainer;
*        typedef list<Widget> WidgetContainer;
* 
*   依赖类型 -- 为了避免潜在的解析含糊，要求在依赖形式类型参数的类型名字之前使用 typename，否则无法编译(常在模版类中出现 -- ItemCache )
*     template<typename C>
*     bool lastGreaterThanFirst(const C& container){ 
*       typename C::const_iterator begin(container, begin());  
*       //C::const_iterator 是依赖于形式类型参数C的类型，行首必须添加 typename
*     }
*
*  模板代码膨胀(code bloat)问题
*    1.gcc 2.96 前的Bug，可以通过 -fmerge-templates，让一个必要的模板实例仅在可执行文件中出现一次；
*    2.内联函数：-finline-limit-50 可以限制内联函数的大小
*
*  Effective STL
*    5.尽量使用区间成员函数代替它们的单元素兄弟 -- 效率和代码简化
*      如使用 vector.assign 代替 循环赋值和 copy 函数
*    7.★当使用new得指针的容器时，记得在销毁容器前delete那些指针 -- 可以使用自定义的 MyDeleteObject 结构★
*    8.永不建立auto_ptr的容器 -- auto_ptr的容器是禁止的(不能编译)
*      拷贝auto_ptr时，auto_ptr所指向对象的所有权被转移到拷贝的auto_ptr，而被拷贝的auto_ptr被设为NULL -- 拷贝一个auto_ptr将改变它的值!
*      
*    9.在删除选项中仔细选择(★删除后迭代器实效★) -- 不同的容器，有不同的删除指定元素的方法(从int型容器中删除通过 badValue 函数返回 true 的元素)
*      连续内存容器删除(erase)后，返回指向紧接在被删元素之后的元素的有效迭代器(i = c.erase(i))；关联容器不返回(需要后置递增)
*      vector、string、deque(连续内存容器) -- c.erase(remove_if(c.begin(), c.end(), badValue),c.end()); //
*      list -- c.remove_if(badValue);
*      set、multiset、map或multimap(标准关联容器) -- c.erase(1963);
*        容易但效率较低 -- 拷贝了所有不删除的元素
*          -- remove_copy_if(c.begin(), c.end(),inserter(goodValues,goodValues.end()),badValue); //从c拷贝不删除的值到goodValues
*          -- c.swap(goodValues);  //交换c和goodValues的内容
*        循环但需注意删除后迭代器失效的问题
*          连续内存容器 -- 用新的迭代器进行更新, iter = xxx.erase(iter)
*          关联容器 -- 后置递增迭代器, xxx.erase(iter++) 
*          具体可见 test_set_loopdelete、test_map_loopdelete 等
*    12.STL容器通常不是线程安全的 -- 不支持读取时写入或多写入
*       a.多个读取者是安全的 --多线程可以同时读取一个容器的内容(读取时不能有任何写入);
*       b.对不同容器的多个写入者是安全的 -- 多线程可以同时写不同的容器
*    13.尽量使用vector和string来代替动态分配的数组 -- 但如果在多线程环境中使用引用计数字符串，就应该注意线程安全性支持所带来的的性能下降问题
*    14.用reserve来避免不必要的重新分配 -- 否则可能会调用 realloc 进行内存分配和拷贝
*       size() -- 返回容器中有多少元素；
*       capacity() -- 已经分配的内存中可以容纳多少元素（包括已分配的和未分配的）；
*       resize() -- 强制把容器改为容纳n个元素(可以增大也可以减小)，其size值会变 -- 增大时会分配对象实例，使用默认值填充；
*       reserve() -- 将最大容量改为至少n个元素(只能增大)，capacity会变；
*    16.将vector和string的数据传给遗留的API（只读，不允许修改，否则“可能”会产生错误 -- void doSomething(const char *pString))
*         &v[0] -- 必须保证 v 不为 empty(否则结果不确定)，vector中的元素被C++标准限定为存储在连续内存中，就像是一个数组
*         s.c_str() -- string不保证连续内存和null结束，因此不能使用类似 vector的方法
*       如果想用C风格API返回的元素初始化一个vector，可以利用vector和数组潜在的内存分布兼容性将存储vecotr的元素的空间传给API函数--但需要小心错误
*         size_t fillArray(double *pArray, size_t arraySize); //填充数组，返回写入的double数
*         vector<double> vd(maxNumDoubles);  //建立一个vector，大小是maxNumDoubles
*         vd.resize(fillArray(&vd[0], vd.size())); //让fillArray把数据写入vd，然后调整vd的大小为fillArray写入的元素个数
*    17.使用“交换技巧”来压缩容器的多余容量 -- 收缩到合适（shrink to fit）
*         //表达式vector<Contestant>(contestants)建立一个临时vector，它是contestants的一份拷贝：vector的拷贝构造函数做了这个工作。
*         //vector的拷贝构造函数只分配拷贝的元素需要的内存，没有多余的容量。然后让临时vector和contestants交换数据。
*         //临时vector被销毁时，释放多余的内存
*         vector<Contestant>(contestants).swap(contestants);
*    18.避免使用vector<bool> -- 不是一个STL容器（伪容器），并不容纳bool(打包bool以节省空间,一个8比特的字节将容纳8个“bool”)
*       //无法编译以下代码： vector<bool> v;bool *pb = &v[0]; //禁止有指向单个比特的指针
*       应该使用 deque<bool> 或 bitset 来代替。
*    19.★了解相等（基于operator==）和等价（通常基于operator<，是基于在一个有序区间中对象值的相对位置）的区别★
*       算法 find 使用 "相等"来查找；set::insert 使用 等价 来判断是否有重复的值
*         忽略大小写的 set<string, CIStringCompare> 中会认为“stl”和“STL”是等价的，但“stl”和“STL”不相等
*         插入“stl”后“STL”就无法插入，然后使用 set::find 方法查找“STL”会成功，但使用标准的find算法则无法查找到"STL"。
*       每个标准关联容器保持有序，通过它的 key_comp 成员函数来访问排序判断式，默认是 less
*       如果下式求值为真，两个对象x和y关于一个关联容器c的排序标准有等价的值：!c.key_comp()(x, y) && !c.key_comp()(y, x)
*         注意：c.key_comp()返回一个函数（或一个函数对象）
*    20.★为指针的关联容器指定比较类型 -- 默认会按照指针地址的顺序进行排序★
*         如 set<string*> 定义的容器不能对string 进行正确排序(等价于 set<string*, less<string*> >)。
*         应更改为 set<string*, StringPtrLess>;
*    21.(关联容器中)永远让比较函数对相等的值返回false（比较函数的实质是比较是否小/(或大)于）
*         -- 否则在判断相同的元素时，会认为是不等价的。
*    22.避免原地修改set和multiset的键(影响容器有序性的元素部分)--如果要更改非KEY部分，需要使用const转换为引用，const_cast<T&>(*i).
*         所有标准关联容器需要保持元素有序(map/multimap 不能编译原地修改的代码 -- const)
*         如果需要更改，必须采用 “定位数据[iter=s.find(..),if(iter!=se.end())]->生成副本(T t(*iter))->修改副本(t.xxx)
*           ->删除原数据(s.erase(iter++))->插入元数据(s.insert(iter,t))”的方式
*    23.考虑用有序vector代替关联容器 -- 当对数据的使用可以明显得区分为 插入、查找和删除 等不同的阶段。
*         标准关联容器的典型实现是平衡二叉查找树（插入、删除和查找的混合操作优化），用于混合插入、删除、查找操作时。
*    26.尽量用 iterator 代替 const_iterator，reverse_iterator 和 const_reverse_iterator
*       每个标准容器类都提供四种迭代器类型，但有些成员函数(如 insert 和 erase)只接收 iterator 作为参数
*    27.用distance和advance把const_iterator转化成iterator -- 必须能访问所指的容器
*         通常iterator和const_iterator是完全不同的类，因此不能使用强制类型转换(const_cast)
*         应该使用：advance(i, distance<ConstIter>(i, ci));
*         效率：对于随机访问的迭代器是常数时间的操作，对于双向迭代器是线性时间的操作
*    28.可以通过reverse_iterator的base得到iterator
*         对于insert操作而言，ri和ri.base()是等价的，而且ri.base()真的是ri对应的iterator；
*         对于删除操作而言，ri和ri.base()并不等价，应该删除ri.base()的前一个元素 -- v.erase((++ri).base());
*    29.需要一个一个字符输入时考虑使用istreambuf_iterator -- 比 istream_iterator 的性能好
*    30.确保目标区间足够大 -- 可以使用多种 inserter 或 调用 resize 方法
*         通常STL容器在被添加时（insert、push_front 等）会自动扩展它们自己来容纳新对象，
*         但有的算法(如 transform )不会对容器自动扩展，因此如下插入代码有错（其中 int transmogrify(int x)）：
*           transform(values.begin(), values.end(),results.end(),transmogrify);
*         应该更改为使用各种 inserter (back_inserter返回的迭代器会调用push_back)：
*           results.reserve(results.size() + values.size()); //可以先调用该函数扩展容量以提高性能(注意：没有扩展大小)
*           transform(values.begin(), values.end(),back_inserter(results),transmogrify);  //或inserter，front_inserter
*         使用transform进行覆盖：
*           if (results.size() < values.size()){results.resize(values.size());} //需要先调用resize保证空间足够大
*           transform(values.begin(), values.end(),results.begin(),transmogrify);
*    32.★如果想删除东西的话就在类似remove的算法后接上成员函数erase★ -- remove算法不知道操作的容器，无法删除，只是将不需要删除的数据移至
*         开始，并返回指向要删除位置的迭代器
*         remove_if 和 unique 使用相同的算法。
*         但list的remove、remove_if和unique成员函数真的去掉了元素，后面不需要接着调用erase。
*    33.★提防在指针的容器上使用类似remove的算法(remove_if/unique)★ -- 需要被删除的指针被覆盖(内存泄漏)
*       可以使用 partition 作为替代
*    37.用accumulate或for_each来统计区间 -- 把整个区间提炼成一个单独的数或一个对象      
*         如 计算一个容器中所有值的和；对一个容器中的字符串长度求和
*    38.把仿函数类设计为用于值传递 -- 应该设计可以拷贝的函数对象
*         C和C++都不允许你真的把函数作为参数传递给其他函数。取而代之的是，你必须传指针给函数。
*         程序员的责任是保证 函数对象以值传递和返回 时行为良好：
*           a. 函数对象应该很小。否则它们的拷贝会很昂贵；
*           b. 函数对象必须单态 -- 不能用虚函数(派生类对象以值传递代入基类类型的参数会造成切割问题--派生部分被删除)
*       ★但可以通过另外创建一个辅助类保存小的、多态函数，然后给仿函数传递该类的指针，来使用大的、支持多态的仿函数对象★
*           -- 参见 CBPFC(即 Bridge 模式) -- 实现和测试有问题，怎么更改？
*    39.用纯函数做判断式 -- 每次判断结果只和参数有关，不能记忆状态或随时间变化（通常把operator()函数声明为const）
*         判断式是返回bool（或者其他可以隐式转化为bool的东西）-- 任何STL想要一个判断式的地方，它都会接受一个真的判断式或一个判断式类对象
*         纯函数是返回值只依赖于参数的函数 -- 不依赖于状态、时间等因素
*    40.★使仿函数类可适配 -- 提供必要的 typedef 的函数对象称为可适配的，可用于更多场合★
*         operator()带一个实参的仿函数类，要从std::unary_function继承  -- unary_function<参数类型, 返回类型>
*         operator()带两个实参的仿函数类，要从std::binary_function继承 -- binary_function<第一参数类型,第二参数类型，返回类型>
*            实际参数不是指针时是 const 参数类型 & （即常引用），实际参数是指针时，传递参数一样 (const CMyTestData*)
*         如 不小于60 可以表示为：not1(bind2nd(less<int>(), 60))
*         算法中使用函数 f 进行计算，为了计算不满足 f 的元素，需要 not1(ptr_func(f))
*         ptr_fun 会使一些 typedef（如argument_type、first_argument_type、second_argument_type 和 result_type 等）
*           变得有效，而 not1 等函数需要这些定义
*    43.★尽量用算法调用代替手写循环★ -- 效率高，不容易产生错误，代码更简介（易维护）
*         a.库的实现者可以利用他们知道容器的具体实现的优势，用库的使用者无法采用的方式来优化遍历
*         b.自定义循环需要考虑 迭代器失效 的问题，算法内部会考虑；
*         c.专业的C++程序员应该知道每个算法都完成了什么（就像UML一样，应该成为基础知识）,算法的名字传达了大量的语义信息
*         如果循环里要做的事非常简单，但调用算法时却需要使用绑定和适配器或者需要独立的仿函数类，你恐怕还是写循环比较好
*    44.尽量用成员函数代替同名的算法 
*         a.效率：set::find 对数时间，而 ::find 需要线性时间；
*         b.成员函数使用“等价”判断，而通用算法使用“相等”判断 -- （条款19）；
*         c.当操纵map和multimap时，成员函数只处理key值，通用算法处理(key, value)对；
*    46.考虑使用函数对象代替函数作算法的参数 -- 效率高，更易编译，更稳健
*         把STL函数对象传递给算法所产生的代码一般比传递真的函数高效 -- 函数对象的operator()函数被声明为内联时
************************************************************************/

#endif //_STL_STUDY_H