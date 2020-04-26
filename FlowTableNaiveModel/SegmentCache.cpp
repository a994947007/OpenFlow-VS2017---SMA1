#include "stdafx.h"
#include "HashFunctions.h"
#include "SegmentCache.h"

template <class T>
SegmentCache<T>::SegmentCache(ULONG row,ULONG col,ULONG segNum,ULONG segSize):AbstractFlowCache(row,col),SEG_NUM(segNum),SEG_SIZE(segSize) {

}

template <class T>
ULONG SegmentCache<T>::FindFlow(const FlowID& fid, const Time& t, T & pFlow) {
	ULONG len = 0;
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	u8 * result = new u8[SEG_NUM * SEG_SIZE];
	hashFunction->compute(buf, FID_LEN, result);
	ULONG sign = BOB(buf, FID_LEN);		//计算签名
	for (ULONG m = 0; m < SEG_NUM; m++) {
		//计算哈希结果对应的索引值大小
		u32 pos = 0x0;
		for (ULONG i = 0; i < SEG_SIZE; i++) {
			pos |= result[m * SEG_SIZE + i];
			pos <<= (i + 1);
		}
		//查找缓存
		for (ULONG j = 0; j < COL_NUM; j++) {
			len++;
			if (sign == cacheTable[pos][j].sign) {
				pFlow = cacheTable[pos][j].entry;
				cacheTable[pos][j].time = t;		//找到则更新时间
				break;
			}
		}
	}
	return len;
}

template <class T>
ULONG SegmentCache<T>::InsertFlow(const FlowID& fid, const Time& t, T pFlow) {
	ULONG len = 0;
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG sign = BOB(buf, FID_LEN);
	u8* result = new u8[SEG_NUM * SEG_SIZE];
	hashFunction->compute(buf, FID_LEN, result);
	u32 insertRow = 0x0;
	u32 insertCol = 0x0;
	for (int m = 0; m < SEG_NUM; m++) {
		//计算哈希结果对应的索引值大小
		u32 pos = 0x0;
		for (ULONG i = 0; i < SEG_SIZE; i++) {
			pos |= result[m * SEG_SIZE + i];
			pos <<= (i + 1);
		}
		insertRow = pos;
		if (m == 0) continue;
		//寻找最久未使用的项
		for (ULONG j = 0; j < COL_NUM; j++) {
			len++;
			if (cacheTable[pos][j].time < cacheTable[insertRow][insertCol].time) {
				insertRow = pos;
				insertCol = j;
			}
		}
	}
	cacheTable[insertRow][insertCol].sign = sign;
	cacheTable[insertRow][insertCol].entry = pFlow;
	cacheTable[insertRow][insertCol].time = t;
	return len;
}
