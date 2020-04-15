#include "stdafx.h"
#include "SegmentHashFunction.h"
#include "cmd5.h"


uint8_t* SegmentHashFunction::compute(uint8_t * src, uint16_t size, uint8_t* dst) {
	MD5Digest((char *)src, size, (char *)dst);
	return dst;
}