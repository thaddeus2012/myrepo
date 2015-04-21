#ifndef CLEXECUTIVEFUNCTIONFORMSGLOOP_H
#define CLEXECUTIVEFUNCTIONFORMSGLOOP_H

#include "CLStatus.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageLoopManager.h"

class CLExecutiveFunctionForMsgLoop: public CLExecutiveFunctionProvider{
    public:
	explicit CLExecutiveFunctionForMsgLoop(CLMessageLoopManager* pMsgLoopManager);
	virtual ~CLExecutiveFunctionForMsgLoop();
	
	virtual CLStatus RunExecutiveFunction(void* pContext);

    private:
	CLExecutiveFunctionForMsgLoop(const CLExecutiveFunctionForMsgLoop&);
	CLExecutiveFunctionForMsgLoop& operator=(const CLExecutiveFunctionForMsgLoop&);

    private:
	CLMessageLoopManager* m_pMsgLoopManager;
};
#endif
