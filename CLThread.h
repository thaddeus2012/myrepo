#ifndef CLTHREAD_H
#define CLTHREAD_H

#include "CLStatus.h"
#include "CLExecutive.h"
#include "CLExecutiveFunctionProvider.h"

class CLThread: public CLExecutive{
    public:
	explicit CLThread(CLExecutiveFunctionProvider*);
	virtual ~CLThread();

	virtual CLStatus Run(void* pContext = 0);
	virtual CLStatus WaitForDeath();

    private:
	static void* StartFunctionOfThread(void* pContext);

    protected:
	pthread_t m_ThreadID;
	void* m_pContext;
};
#endif
