#ifndef FTL_DESIGN_PATTERN_H
#define FTL_DESIGN_PATTERN_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDesignPattern.h requires ftlbase.h to be included first
#endif

#include <list>

/*************************************************************************************************************************
* 一些常见的设计模式模板类
*
*   
*************************************************************************************************************************/

namespace FTL
{
	//观察者模式中被观察对象的基类， T 一般为 IxxxEvent 等接口
	template <class T>
	class CFObservedT 
	{
	public:
		FTLINLINE CFObservedT();
		FTLINLINE virtual ~CFObservedT();
	public:
		typedef std::list<T *> listobserver_t;
		FTLINLINE void GetObservers(listobserver_t *pObservers);
		FTLINLINE virtual void RegisterObserver(T *pOb);
		FTLINLINE virtual void UnRegisterObserver(T *pOb);
		FTLINLINE size_t size(void);
	protected:
		listobserver_t m_listObserver;
	};
}
#endif //FTL_DESIGN_PATTERN_H

#ifndef USE_EXPORT
#  include "ftlDesignPattern.hpp"
#endif 