#ifndef CLTHREAD_H
#define CLTHREAD_H

#include "CLStatus.h"
#include "CLExecutive.h"
#include "CLEvent.h"

class CLThread: public CLExecutive{
    public:
	explicit CLThread(CLCoordinator*);
	CLThread(CLCoordinator*, bool);
	virtual ~CLThread();

	virtual CLStatus Run();
	virtual CLStatus WaitForDeath();

    private:
	static void* StartFunctionOfThread(void* pContext);

    private:
	pthread_t m_ThreadID;
	
	bool m_bThreadCreated; //判断是否创建了线程对象,以免多次调用Run导致创建多个对象
	bool m_bWaitForDeath; //是否要等待新线程死亡

	CLEvent m_EventForWaitingForNewThread;
	CLEvent m_EventForWaitingForOldThread;
};
#endif
