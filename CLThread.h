#ifndef CLTHREAD_H
#define CLTHREAD_H

#include "CLStatus.h"
#include "CLExcutive.h"
#include "CLExcutiveFunctionProvider.h"

class CLThread: public CLExcutive{
    public:
	explicit CLThread(CLExcutiveFunctionProvider*);
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
