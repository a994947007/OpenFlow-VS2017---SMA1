#pragma once

#include "uniform.h"
#include "HashTable.h"

#include <list>
#include <fstream>

using namespace std;

//�����������
static const USHORT MASK_NUM_ROOT = 4;
static const USHORT MASK_NUM = MASK_NUM_ROOT * MASK_NUM_ROOT;
static const USHORT MASK_PROTO = 0xff;
static const USHORT MASK_PORT = 0x8000;

//��������
static const UINT MASK_LIST[] = { 0xff000000,0xfff00000,0xffff0000,0xffffff00 };

/* �û�������Ϣ */
typedef struct UserConfig
{
	list<CString> fileList;
	CString resultPath;
	UINT	uiHashLen;	//�����ϣ����
	UserConfig() : uiHashLen(0)
	{

	}
	~UserConfig()
	{

	}
}UserConfig, *pUserConfig;

class PacketLookup
{
public:
	PacketLookup(const UserConfig &);
	virtual ~PacketLookup();

	void LookupTestProc(const UserConfig &);

protected:
	bool Initialize(const UserConfig &);
	bool GetPacket(Packet &);
	bool ReadPacket(ifstream &, Packet &);

	bool MaskProbeNaive(const Packet &);

	const FlowID CalcFlowID(UCHAR, const EndID &, const EndID &);
	Flow*	NewFlow(const Packet &, const FlowID &);
	UINT	GetIpMask(UINT ip);
	bool	UpdateFlow(Flow *, const Packet &, const FlowID &);
	bool	WriteResult(bool, const UserConfig &);
	void	Clear(list<ifstream*> &);
	ULONG	GetFlowNum(HashTable **);

protected:
	list<ifstream *> m_fileStream;		//TRACE�ļ���
	HashTable * m_tableNaive[MASK_NUM];	

	ULONG m_ulLookupNum,m_ulLookupLenNaive;
	ULONG m_ulProbeNumNaive;
};

