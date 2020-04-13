#include "stdafx.h"
#include "HashFunctions.h"
#include "SegmentCache.h"

SegmentCache::SegmentCache(ULONG row,ULONG col):AbstractFlowCache(row,col) {

}

ULONG SegmentCache::FindFlow(const FlowID& fid, const Time& t, PFlow& pFlow) {
	ULONG len = 0;
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	u8 * result = new u8[SEG_NUM * SEG_SIZE];
	hashFunction->compute(buf, FID_LEN, result);
	ULONG sign = BOB(buf, FID_LEN);		//����ǩ��
	for (ULONG m = 0; m < SEG_NUM; m++) {
		//�����ϣ�����Ӧ������ֵ��С
		u32 pos = 0x0;
		for (ULONG i = 0; i < SEG_SIZE; i++) {
			pos |= result[m * SEG_SIZE + i];
			pos <<= (i + 1);
		}
		//���һ���
		for (ULONG j = 0; j < COL_NUM; j++) {
			len++;
			if (sign == cacheTable[pos][j].sign) {
				pFlow = cacheTable[pos][j].entry;
				cacheTable[pos][j].time = t;		//�ҵ������ʱ��
				break;
			}
		}
	}
	return len;
}


ULONG SegmentCache::InsertFlow(const FlowID& fid, const Time& t, PFlow pFlow) {
	ULONG len = 0;
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG sign = BOB(buf, FID_LEN);
	u8* result = new u8[SEG_NUM * SEG_SIZE];
	hashFunction->compute(buf, FID_LEN, result);
	u32 insertRow = 0x0;
	u32 insertCol = 0x0;
	for (int m = 0; m < SEG_NUM; m++) {
		//�����ϣ�����Ӧ������ֵ��С
		u32 pos = 0x0;
		for (ULONG i = 0; i < SEG_SIZE; i++) {
			pos |= result[m * SEG_SIZE + i];
			pos <<= (i + 1);
		}
		insertRow = pos;
		if (m == 0) continue;
		//Ѱ�����δʹ�õ���
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
