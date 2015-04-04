#ifndef CLTHREAD_H
#define CLTHREAD_H

#include "CLStatus.h"
#include "CLExecutive.h"

class CLThread: public CLExecutive{
    public:
	explicit CLThread(CLCoordinator*);
	virtual ~CLThread();

	virtual CLStatus Run();
	virtual CLStatus WaitForDeath();

    private:
	static void* StartFunctionOfThread(void* pContext);

    protected:
	pthread_t m_ThreadID;
};
#endif
