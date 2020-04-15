#pragma once

#include "uniform.h"
#include "FlowCache.h"
/**
*	����ĳ�����
*/
class AbstractFlowCache : public FlowCache
{
protected:
	//������
	typedef struct CacheEntry {
		ULONG sign;		//����ǩ��
		Flow* entry;	//������ָ��
		Time time;		//������ʰ���ʱ��

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
	//���������������
	ULONG ROW_NUM, COL_NUM;
	//��ǰ�������Ѿ����ڵĻ�������
	ULONG m_ulCurNum;
	//�����
	CacheEntry** cacheTable;

protected:
	AbstractFlowCache(ULONG,ULONG);
	virtual ~AbstractFlowCache();
};