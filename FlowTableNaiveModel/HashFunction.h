#pragma once

#include "uniform.h"

/**
*	哈希函数定义模板
*/
class HashFunction {
public:
	/**
	*	哈希函数模板
	*	@arg1 待计算串
	*	@arg2 待计算串长度
	*	@arg3 计算结果
	*	@return 计算结果
	*/
	virtual uint8_t * compute(uint8_t*, uint16_t,uint8_t *) = 0;
};