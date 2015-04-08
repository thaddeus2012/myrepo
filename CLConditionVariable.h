#ifndef CLCONDITIONVARIABLE_H
#define CLCONDITIONVARIABLE_H

#include "CLStatus.h"
#include "CLLogger.h"
#include "CLMutex.h"

class CLConditionVariable{
    public:
	CLConditionVariable();
	virtual ~CLConditionVariable();

	CLStatus Wait(CLMutex* pMutex);
	CLStatus Wakeup();
	CLStatus WakeupAll();

    private:
	pthread_cond_t m_ConditionVariable;
};
#endif
