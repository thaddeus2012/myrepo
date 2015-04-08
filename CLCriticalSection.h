#ifndef CLCRITICALSECTION_H
#define CLCRITICALSECTION_H

#include "CLStatus.h"
#include "CLLogger.h"
#include "CLMutex.h"

class CLCriticalSection{
    public:
	CLCriticalSection(CLMutex* pMutex);
	virtual ~CLCriticalSection();

    private:
	CLCriticalSection(CLCriticalSection&);
	CLCriticalSection& operator=(const CLCriticalSection&);

    private:
	CLMutex* m_pMutex;
};
#endif
