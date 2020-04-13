#pragma once

#include "AbstractFlowCache.h"
#include "HashFunction.h"
/**
*	�ֶλ���
*/
class SegmentCache : public AbstractFlowCache
{
public:
	SegmentCache(ULONG, ULONG);

	/**
	*	���һ���
	*	ʹ����������������Ĺ�ϣ����������������ʹ�ù�ϣ�����и����ӳ�䷶Χ
	*	����������Ȼ����32λ�Ĺ�ϣֵ
	*/
	ULONG FindFlow(const FlowID&, const Time&, PFlow&);

	ULONG InsertFlow(const FlowID&, const Time&, PFlow);

protected:
	ULONG SEG_NUM;		//��ϣֵ�ķֶ���
	ULONG SEG_SIZE;		//�ֶεĴ�С����λ���ֽڣ�
	HashFunction * hashFunction;	//��ϣ��������
};

