#pragma once

#include "uniform.h"

/**
* 缓存抽象模板，定义所有缓存可操作方法
* 所有OF缓存均应该实现该接口
*/
class FlowCache 
{
public:
	/**
	*	查找缓存
	*	@arg1	流ID
	*	@arg2	当前查询时间
	*	@arg3	掩码索引
	*	@return 操作次数
	*/
	virtual ULONG FindFlow(const FlowID&, const Time&, PFlow&) = 0;
	/**
	*	插入一条流到缓存
	*	@arg1	流ID
	*	@arg2	当前插入时间
	*	@arg3	待插入流
	*	@return	操作次数
	*/
	virtual ULONG InsertFlow(const FlowID&, const Time&, PFlow) = 0;
	/**
	*	插入一条流到缓存
	*	@arg1	流ID
	*	@arg2	当前插入时间
	*	@arg3	掩码索引/元组编号
	*	@return	踢除次数
	*/
	virtual ULONG InsertIndex(const FlowID&, const Time&, ULONG) = 0;
	/**
	*	超时扫描函数
	*	@arg1	当前扫描时间
	*	@return	扫描状态
	*/
	virtual bool TimeoutScan(const Time &) = 0;
	/**
	*	获取当前缓存的利用率
	*	@return 缓存利用率
	*/
	virtual double GetUSsage() = 0;
};