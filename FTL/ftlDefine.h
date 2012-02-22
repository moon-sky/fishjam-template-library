#ifndef FTL_DEFINE_H
#define FTL_DEFINE_H
#pragma once

namespace FTL
{
    //使用模版的export关键字，将声明和实现分开 -- 但目前VS2003~VS2008都不支持 export 关键字
    #ifdef USE_EXPORT
    #  define FTLEXPORT  export
    #  define FTLINLINE
    #else
    #  define FTLEXPORT
    #  define FTLINLINE inline
    #endif

    //调试版本下自动定义 FTL_DEBUG，RELEASE 版本下可以手工定义 FTL_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
    #  ifndef FTL_DEBUG
    #    define FTL_DEBUG
    #  endif
    #endif

    #ifndef QQUOTE
    #  define    QUOTE(x)        #x
    #  define    QQUOTE(y)       QUOTE(y)
    #  define     __FILE__LINE__  TEXT(__FILE__) TEXT("(") TEXT(QQUOTE(__LINE__)) TEXT(") :")
    #endif //QQUOTE

    #define JOIN_ONE(x, y) x ## y
    #define JOIN_TWO(x, y) JOIN_ONE(x, y)

    //用于在编译器的输出窗口(Output)中输出“TODO”信息，可以双击直接进行定位
    //使用方式：pragma TODO(Fix this later)
    #define TODO(desc) message(__FILE__ "(" QQUOTE(__LINE__) ") : TODO: " #desc)

    //定义来查找一个数组中的 past-the-end 位置，使用方式:
    //  std::find(intArray, ARRAY_PAST_THE_END(intArray), 10);
    #define ARRAY_PAST_THE_END(c) ( c + _countof( c ) )

    //用于<重载时比较两个成员的对象变量
    #define COMPARE_OBJ_LESS(f,l,r) \
        if( l.f < r.f ) { return true; } \
        else if( l.f > r.f ) { return false; }
        
    //定义比较成员变量的宏，常用于重载类、结构的 operator < 时（注意：最后一个变量时），参数分别为 field , other&
    #define COMPARE_MEM_LESS(f, o) \
        if( f < o.f ) { return true; }\
        else if( f > o.f ) { return false; }

}


#endif //FTL_DEFINE_H