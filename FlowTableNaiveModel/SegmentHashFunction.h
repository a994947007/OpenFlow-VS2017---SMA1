#pragma once
#include "HashFunction.h"
class SegmentHashFunction :public HashFunction
{
	uint8_t* compute(uint8_t*, uint16_t, uint8_t*);
};

