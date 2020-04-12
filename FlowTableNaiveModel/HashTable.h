#pragma once

#include "uniform.h"
#include "Hashlist.h"
#include <vector>

using namespace std;

class HashTable
{
public:
	HashTable(UINT);
	virtual ~HashTable();

	ULONG Find(const FlowID &, PFlow &);
	bool Insert(Flow *);
	void Free();

	ULONG GetTotalNum();
	ULONG TimeoutScan(Time);
	void SetMask(const FlowID &);

protected:
	USHORT CalcHash(const FlowID &);

protected:
	const UINT HASH_LEN;//��ϣ����
	vector<HashList *> m_pHashTable;	//��ϣ��

public:
	FlowID mask;		//��ǰԪ���Ӧ������
	ULONG ulPktNum;		//��ǰԪ����ʴ���
	Time tLastPkt;		//���һ�η��������ʱ��


};

