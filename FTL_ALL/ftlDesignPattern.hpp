#ifndef FTL_DESIGN_PATTERN_HPP
#define FTL_DESIGN_PATTERN_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDesignPattern.h"
#endif

namespace FTL
{
	template<typename T>
	CFObservedT<T>::CFObservedT()
	{
	}

	template<typename T>
	CFObservedT<T>::~CFObservedT()
	{
	}

	template<typename T>
	void CFObservedT<T>::GetObservers(listobserver_t *pObservers)
	{
		*pObservers = m_listObserver;
	}

	template<typename T>
	void CFObservedT<T>::RegisterObserver(T *pOb)
	{
		m_listObserver.remove(pOb);
		m_listObserver.push_back(pOb);
	}

	template<typename T>
	void CFObservedT<T>::UnRegisterObserver(T *pOb)
	{
		m_listObserver.remove(pOb);
	}

	template<typename T>
	size_t CFObservedT<T>::size(void)
	{
		return m_listObserver.size();
	}
}
#endif //FTL_DESIGN_PATTERN_HPP