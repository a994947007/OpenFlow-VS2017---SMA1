#include "stdafx.h"
#include "AbstractFlowCache.h"

template <class T>
AbstractFlowCache<T>::AbstractFlowCache(ULONG row, ULONG col) :ROW_NUM(row), COL_NUM(col),m_ulCurNum(0)
{
	cacheTable = new CacheEntry * [row];
	for (ULONG i = 0; i < row; i++)
	{
		for (ULONG j = 0; j < col; j++) {
			cacheTable[i] = new CacheEntry[col];
		}
	}
}

template <class T>
AbstractFlowCache<T>::~AbstractFlowCache()
{
	for (ULONG i = 0; i < ROW_NUM; i++)
	{
		if (cacheTable[i]) {
			delete [] cacheTable;
		}
	}
	delete [] cacheTable;
}
