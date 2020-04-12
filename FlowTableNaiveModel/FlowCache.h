#pragma once

#include "uniform.h"

/**
* �������ģ�壬�������л���ɲ�������
* ����OF�����Ӧ��ʵ�ָýӿ�
*/
class FlowCache 
{
public:
	/**
	*	���һ���
	*	@arg1	��ID
	*	@arg2	��ǰ��ѯʱ��
	*	@arg3	��������
	*	@return ��������
	*/
	virtual ULONG FindFlow(const FlowID&, const Time&, PFlow&) = 0;
	/**
	*	����һ����������
	*	@arg1	��ID
	*	@arg2	��ǰ����ʱ��
	*	@arg3	��������
	*	@return	��������
	*/
	virtual ULONG InsertFlow(const FlowID&, const Time&, PFlow) = 0;
	/**
	*	����һ����������
	*	@arg1	��ID
	*	@arg2	��ǰ����ʱ��
	*	@arg3	��������/Ԫ����
	*	@return	�߳�����
	*/
	virtual ULONG InsertIndex(const FlowID&, const Time&, ULONG) = 0;
	/**
	*	��ʱɨ�躯��
	*	@arg1	��ǰɨ��ʱ��
	*	@return	ɨ��״̬
	*/
	virtual bool TimeoutScan(const Time &) = 0;
	/**
	*	��ȡ��ǰ�����������
	*	@return ����������
	*/
	virtual double GetUSsage() = 0;
};