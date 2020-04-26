#pragma once

#include "AbstractFlowCache.h"
#include "HashFunction.h"
/**
*	分段缓存
*/
template <class T>
class SegmentCache : public AbstractFlowCache<T>
{
public:
	SegmentCache(ULONG, ULONG,ULONG,ULONG);

	/**
	*	查找缓存
	*	使用其他运算结果更宽的哈希函数来生成索引，使得哈希函数有更宽的映射范围
	*	缓存项中仍然保存32位的哈希值
	*/
	ULONG FindFlow(const FlowID&, const Time&, T&);

	ULONG InsertFlow(const FlowID&, const Time&, T);

protected:
	ULONG SEG_NUM;		//哈希值的分段数
	ULONG SEG_SIZE;		//分段的大小，单位（字节）
	HashFunction * hashFunction;	//哈希函数对象
};

