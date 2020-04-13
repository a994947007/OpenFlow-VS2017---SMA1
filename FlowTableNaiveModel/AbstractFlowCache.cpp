#include "stdafx.h"
#include "AbstractFlowCache.h"

AbstractFlowCache::AbstractFlowCache(ULONG row, ULONG col) :ROW_NUM(row), COL_NUM(col),m_ulCurNum(0)
{
	cacheTable = new CacheEntry * [row];
	for (ULONG i = 0; i < row; i++)
	{
		for (ULONG j = 0; j < col; j++) {
			cacheTable[i] = new CacheEntry[col];
		}
	}
}

AbstractFlowCache::~AbstractFlowCache()
{
	for (ULONG i = 0; i < ROW_NUM; i++)
	{
		if (cacheTable[i]) {
			delete [] cacheTable;
		}
	}
	delete [] cacheTable;
}
