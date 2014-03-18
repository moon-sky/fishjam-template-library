#ifndef _UHTTP_DEF_H_
#define _UHTTP_DEF_H_

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

#ifdef WIN32
#  ifndef snprintf
#  define snprintf _snprintf
#  endif //snprintf
#endif 

#ifndef JOIN_ONE
#  define JOIN_ONE(x, y) x ## y
#  define JOIN_TWO(x, y) JOIN_ONE(x, y)
#  define FTL_JOIN(x, y) JOIN_TWO(x, y)
#  define FTL_MAKE_UNIQUE_NAME( prefix )  FTL_JOIN( prefix, __LINE__ )
#  define TODO(desc) message(__FILE__ "(" QQUOTE(__LINE__) ") : TODO: " #desc)
#endif 

#endif //_UHTTP_DEF_H_