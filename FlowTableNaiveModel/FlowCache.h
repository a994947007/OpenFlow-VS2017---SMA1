#pragma once

#include "uniform.h"

/**
* �������ģ�壬�������л���ɲ�������
* ����OF�����Ӧ��ʵ�ָýӿ�
*/
template <class T>
class FlowCache 
{
public:
	/**
	*	���һ���
	*	@arg1	��ID
	*	@arg2	��ǰ��ѯʱ��
	*	@arg3	������
	*	@return ��������
	*/
	virtual ULONG Find(const FlowID&, const Time&, T&) = 0;
	/**
	*	����һ����������
	*	@arg1	��ID
	*	@arg2	��ǰ����ʱ��
	*	@arg3	�����뻺����
	*	@return	��������
	*/
	virtual ULONG Insert(const FlowID&, const Time&, T*) = 0;
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