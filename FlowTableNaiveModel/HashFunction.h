#pragma once

#include "uniform.h"

/**
*	��ϣ��������ģ��
*/
class HashFunction {
public:
	/**
	*	��ϣ����ģ��
	*	@arg1 �����㴮
	*	@arg2 �����㴮����
	*	@arg3 ������
	*	@return ������
	*/
	virtual uint8_t * compute(uint8_t*, uint16_t,uint8_t *) = 0;
};