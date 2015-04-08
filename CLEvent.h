#ifndef CLEVENT_H
#define CLEVENT_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

class CLEvent{
    public:
	CLEvent();
	virtual ~CLEvent();

	CLStatus Set();
	CLStatus Wait();

private:
	CLEvent(const CLEvent&);
	CLEvent& operator=(const CLEvent&);

    private:
	CLMutex m_Mutex;
	CLConditionVariable m_Cond;
	int m_Flag;
};
#endif
