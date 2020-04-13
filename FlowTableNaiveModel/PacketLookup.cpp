#include "stdafx.h"
#include "PacketLookup.h"
#include "Log.h"
#include <time.h>


PacketLookup::PacketLookup(const UserConfig & info) :
	m_ulLookupNum(0),m_ulLookupLenNaive(0),
	m_ulProbeNumNaive(0)
{
	srand(unsigned(time(NULL)));

	for (int i = 0; i < MASK_NUM; i++) {
		m_tableNaive[i] = new HashTable(info.uiHashLen);
	}
}

PacketLookup::~PacketLookup()
{
	for (int i = 0; i < MASK_NUM; i++) {
		if (m_tableNaive[i]) {
			delete m_tableNaive[i];
			m_tableNaive[i] = NULL;
		}
	}
}

bool PacketLookup::Initialize(const UserConfig & info) {
	ASSERT(!info.fileList.empty());

	list<CString> strList = info.fileList;
	list<CString>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		ifstream *pFile = new ifstream;
		pFile->open(*iter, ios_base::in | ios_base::binary);
		if (pFile->fail())
		{
			LOG("open file failure!");
			return false;
		}

		m_fileStream.push_back(pFile);
	}

	//��ʼ��Ԫ������
	for (int i = 0; i < MASK_NUM_ROOT; i++) {
		for (int j = 0; j < MASK_NUM_ROOT; j++) {
			FlowID mask;
			mask.proto = MASK_PROTO;
			mask.src.ip = MASK_LIST[i];
			mask.src.port = MASK_PORT;
			mask.dst.ip = MASK_LIST[j];
			mask.dst.port = MASK_PORT;
			m_tableNaive[i * MASK_NUM_ROOT + j]->SetMask(mask);
		}
	}
	return true;
}

void PacketLookup::LookupTestProc(const UserConfig & info) {

	bool blSuccess = Initialize(info);
	if (!blSuccess)
	{
		LOG("Initiate members failed!");
		return;
	}

	WriteResult(false, info);

	bool bIsFirst = true;
	Time tLastScan,tLastWrite;

	Packet pkt;
	while (GetPacket(pkt)) {
		m_ulLookupNum++;

		MaskProbeNaive(pkt);

		if (bIsFirst) {
			tLastWrite = tLastScan = pkt.time;
			bIsFirst = false;
		}
		else {
			Time t = pkt.time - tLastWrite;
			if (t.sec >= 1) {
				WriteResult(true, info);
				tLastWrite = pkt.time;

				m_ulLookupNum = 0;

				m_ulLookupLenNaive = 0;

				m_ulProbeNumNaive = 0;
			}

			t = pkt.time - tLastScan;

			if (t.usec >= SCAN_INTERVAL) {
				for (int i = 0; i < MASK_NUM; i++) {
					m_tableNaive[i]->TimeoutScan(pkt.time);
				}

				tLastScan = pkt.time;
			}
		}
	}
}

bool PacketLookup::MaskProbeNaive(const Packet & pkt) {
	ASSERT(pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP);

	const FlowID fid = CalcFlowID(pkt.proto, pkt.src, pkt.dst);
	Flow *pNode = NULL;

	int index = INDEX_INVALID;

	for (int i = 0; i < MASK_NUM; i++) {
		m_ulLookupLenNaive += m_tableNaive[i]->Find(fid, pNode);
		if (pNode != NULL) {
			index = i;
			break;
		}
		m_ulProbeNumNaive++;
	}

	if (pNode == NULL) {
		pNode = NewFlow(pkt, fid);
		for (int j = 0; j<MASK_NUM; j++) {
			if (pNode->mask == m_tableNaive[j]->mask) {
				index = j;
				break;
			}
		}
	}

	UpdateFlow(pNode, pkt, fid);
	m_tableNaive[index]->Insert(pNode);

	return true;
}

//��������ʶ��
const FlowID PacketLookup::CalcFlowID(UCHAR proto, const EndID & src, const EndID & dst) {
	FlowID fid;
	fid.proto = proto;
	fid.src = src;
	fid.dst = dst;
	
	return fid;
}

//�½�������
Flow * PacketLookup::NewFlow(const Packet & pkt, const FlowID & fid) { 
	Flow * pNew = new Flow;
	if (NULL == pNew) {
		LOG("new flow error");
		return pNew;
	}

	pNew->mask.proto = MASK_PROTO;
	pNew->mask.src.ip = GetIpMask(fid.src.ip);
	pNew->mask.src.port = MASK_PORT;
	pNew->mask.dst.ip = GetIpMask(fid.dst.ip);
	pNew->mask.dst.port = MASK_PORT;
	pNew->key = (FlowID)fid&pNew->mask;
	
	return pNew;
}

//��ȡ��ӦIP��ַ�Ķ�Ӧ���룬��ǰ�����ֽڽ��з���
UINT PacketLookup::GetIpMask(UINT ip)
{
	if (0x00000000 == (ip & 0xC0000000))		//00���ַ
		return MASK_LIST[0];
	else if (0x40000000 == (ip & 0xC0000000)) {	//01���ַ
		return MASK_LIST[1];
	}
	else if (0x80000000 == (ip & 0xC0000000))	//10���ַ
		return MASK_LIST[2];
	else									//11���ַ
		return MASK_LIST[3];
}

bool PacketLookup::UpdateFlow(Flow * pNode, const Packet & pkt, const FlowID & fid) {
	ASSERT(pNode != NULL && (pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP));
	pNode->tLast = pkt.time;
	return true;
}

bool PacketLookup::WriteResult(bool bIsOn, const UserConfig & info) {
	CString strFile = info.resultPath + "/" + "Result.txt";
	ofstream outFile;
	if (!bIsOn) {
		outFile.open(strFile, ios_base::out | ios_base::trunc | ios_base::binary);
		if (!outFile) {
			LOG("OpenFile failure");
			return false;
		}
		outFile << info.uiHashLen << "\r\n";
	}
	else {
		outFile.open(strFile, ios_base::in | ios_base::app | ios_base::binary);
		if (!outFile)
		{
			LOG("OpenFile failure\n");
			return false;
		}

		double dAslNaive = (double)m_ulLookupLenNaive / m_ulLookupNum;
		double dProbeNumNaive = (double)m_ulProbeNumNaive / m_ulLookupNum;

		outFile	<< GetFlowNum(m_tableNaive) << "\t" 
				<< dAslNaive << "\t" 
				<< dProbeNumNaive 
				<< "\r\n";
	}

	outFile.close();
	outFile.clear();
	return true;
}

//��ȡһ�����ݰ�
bool PacketLookup::GetPacket(Packet &pkt)
{
	while (!m_fileStream.empty()) {
		ifstream * f = m_fileStream.front();
		while (!f->eof())
		{
			bool bRet = ReadPacket(*f, pkt);
			if (!bRet)
			{
				LOG("read packet error!");
				continue;
			}
			return true;
		}
		f->close();
		f->clear();
		m_fileStream.pop_front();
		delete f;
		f = NULL;
	}
	return false;
}

ULONG PacketLookup::GetFlowNum(HashTable ** tuples) {
	int count = 0;
	for (int i = 0; i<MASK_NUM; i++) {
		count += tuples[i]->GetTotalNum();
	}
	return count;
}

//��ָ���ļ��ж�ȡһ�����ݰ�
bool PacketLookup::ReadPacket(ifstream &ifFile, Packet &pkt)
{
	//��ȡ���ݰ���ʱ���
	ifFile.read((char *)&(pkt.time), sizeof(pkt.time));
	if (!ifFile)
	{
		TRACE("read timestamp failure!\n");
		return false;
	}

	//��ȡIP�ײ�����
	IpHeader *pIpHeader = (IpHeader *) new char[sizeof(IpHeader)];
	if (NULL == pIpHeader)
	{
		TRACE("new array error!");
		return false;
	}

	ifFile.read((char *)pIpHeader, sizeof(IpHeader));
	if (!ifFile)
	{
		TRACE("read IP header failure!\n");
		return false;
	}

	//��ȡIP�ײ��ؼ��ֶ���Ϣ
	USHORT usIpHdrLen = (pIpHeader->ver_ihl & 0x0f) * 4;//��ȡIPͷ������
	USHORT usIpTotalLen = ntohs(pIpHeader->total_len); 	//��ȡIP���ܳ���
	pkt.src.ip = ntohl(pIpHeader->srcIP);
	pkt.dst.ip = ntohl(pIpHeader->dstIP);
	pkt.proto = pIpHeader->protocol;

	delete[] pIpHeader;
	pIpHeader = NULL;

	//����IPѡ�����
	if (usIpHdrLen > 20)
	{
		ifFile.seekg(4, ios_base::cur);
		if (!ifFile)
		{
			TRACE("seek file failure!\n");
			return false;
		}
	}

	//����Э���ֶ�������ȡ���������
	if (pkt.proto == PROTO_TCP)
	{
		//��ȡTCP�ײ�����
		TcpHeader *pTcpHeader = (TcpHeader *) new char[sizeof(TcpHeader)];
		if (NULL == pTcpHeader)
		{
			TRACE("new array error!");
			return false;
		}

		ifFile.read((char *)pTcpHeader, sizeof(TcpHeader));
		if (!ifFile)
		{
			TRACE("read TCP header failure!\n");
			return false;
		}

		//��ȡ�˿ں�
		pkt.src.port = ntohs(pTcpHeader->src_port);
		pkt.dst.port = ntohs(pTcpHeader->dst_port);

		delete[] pTcpHeader;
		pTcpHeader = NULL;
	}
	else if (pkt.proto == PROTO_UDP)
	{
		//��ȡUDP�ײ�����
		UdpHeader *pUdpHeader = (UdpHeader *)new char[sizeof(UdpHeader)];
		if (NULL == pUdpHeader)
		{
			TRACE("new array error!");
			return false;
		}

		ifFile.read((char *)pUdpHeader, sizeof(UdpHeader));
		if (!ifFile)
		{
			TRACE("read UDP Header Failure!\n");
			return false;
		}

		//��ȡ�˿ں�
		pkt.src.port = ntohs(pUdpHeader->src_port);
		pkt.dst.port = ntohs(pUdpHeader->dst_port);

		delete[] pUdpHeader;
		pUdpHeader = NULL;
	}
	else	//��TCP��UDP����GRE(47)
	{
		ifFile.seekg(8, ios_base::cur);
		if (!ifFile)
			TRACE("seek file failure!\n");

		return false;
	}

	return true;
}