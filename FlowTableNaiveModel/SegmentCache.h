#pragma once

#include "AbstractFlowCache.h"
class SegmentCache : public AbstractFlowCache
{
public:
	SegmentCache(ULONG, ULONG);

	ULONG FindFlow(const FlowID&, const Time&, PFlow&);

protected:
	
};

