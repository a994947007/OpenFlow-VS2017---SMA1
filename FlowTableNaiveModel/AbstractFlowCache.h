#pragma once

#include "uniform.h"
#include "FlowCache.h"
/**
*	缓存的抽象类
*/
class AbstractFlowCache : public FlowCache
{
protected:
	//缓存项
	typedef struct CacheEntry {
		ULONG sign;		//连接签名
		Flow* entry;	//流表项指针
		Time time;		//最近访问包的时间

		CacheEntry() :sign(0), entry(NULL), time(0, 0) {
		}

		inline CacheEntry operator=(const CacheEntry& ce) {
			sign = ce.sign;
			entry = ce.entry;
			time = ce.time;
			return *this;
		}

		inline bool IsEmpty() {
			return sign == 0 && entry == NULL && time.sec == 0 && time.usec == 0;
		}

		inline bool Reset() {
			sign = 0;
			entry = NULL;
			time.Reset();
			return true;
		}
	} CacheEntry, * PCacheEntry;

protected:
	//缓存的行数与列数
	ULONG ROW_NUM, COL_NUM;
	//当前缓存中已经存在的缓存项数
	ULONG m_ulCurNum;
	//缓存表
	CacheEntry** cacheTable;

protected:
	AbstractFlowCache(ULONG,ULONG);
	virtual ~AbstractFlowCache();
};