#ifndef CLMUTEX_H
#define CLMUTEX_H

#include <pthread.h>
#include "CLStatus.h"
#include "CLLogger.h"

class CLMutex{
    public:
	CLMutex();
	virtual ~CLMutex();

	CLStatus Lock();
	CLStatus UnLock();

    private:
	CLMutex(const CLMutex&);
	CLMutex& operator=(const CLMutex&);

    private:
	pthread_mutex_t m_Mutex;
};
#endif
