#pragma once

#include "AbstractFlowCache.h"
#include "HashFunction.h"
/**
*	�ֶλ���
*/
template <class T>
class SegmentCache : public AbstractFlowCache<T>
{
public:
	SegmentCache(ULONG, ULONG,ULONG,ULONG);

	/**
	*	���һ���
	*	ʹ����������������Ĺ�ϣ����������������ʹ�ù�ϣ�����и����ӳ�䷶Χ
	*	����������Ȼ����32λ�Ĺ�ϣֵ
	*/
	ULONG FindFlow(const FlowID&, const Time&, T&);

	ULONG InsertFlow(const FlowID&, const Time&, T);

protected:
	ULONG SEG_NUM;		//��ϣֵ�ķֶ���
	ULONG SEG_SIZE;		//�ֶεĴ�С����λ���ֽڣ�
	HashFunction * hashFunction;	//��ϣ��������
};

