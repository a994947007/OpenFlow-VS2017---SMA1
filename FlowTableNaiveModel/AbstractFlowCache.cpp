#include "stdafx.h"
#include "AbstractFlowCache.h"

AbstractFlowCache::AbstractFlowCache(ULONG row, ULONG col) :ROW_NUM(row), COL_NUM(col),m_ulCurNum(0)
{
	cacheEntry = new CacheEntry * [row];
	for (ULONG i = 0; i < row; i++)
	{
		for (ULONG j = 0; j < col; j++) {
			cacheEntry[i] = new CacheEntry[col];
		}
	}
}

AbstractFlowCache::~AbstractFlowCache()
{
	for (ULONG i = 0; i < ROW_NUM; i++)
	{
		if (cacheEntry[i]) {
			delete [] cacheEntry;
		}
	}
	delete [] cacheEntry;
}
