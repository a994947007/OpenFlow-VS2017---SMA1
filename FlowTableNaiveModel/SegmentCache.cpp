#include "stdafx.h"
#include "HashFunctions.h"
#include "SegmentCache.h"

SegmentCache::SegmentCache(ULONG row,ULONG col):AbstractFlowCache(row,col) {

}

ULONG SegmentCache::FindFlow(const FlowID& fid, const Time& t, PFlow& pFlow) {
	ULONG len = 0;
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG ulFidHash = BOB(buf, FID_LEN);
	//查找缓存
	for (int m = 0; m < sizeof(ulFidHash); m++) {
		UCHAR pos = (ulFidHash >> (m * 8)) & 0xff;
		len++;
		for (ULONG i = 0; i < COL_NUM; i++)
		{
			if (ulFidHash == cacheEntry[pos][i].sign) {
				pFlow = cacheEntry[pos][i].entry;
				cacheEntry[pos][i].time = t;		//找到则更新时间
				break;
			}
		}
	}
	return len;
}
