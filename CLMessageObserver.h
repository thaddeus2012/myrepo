#ifndef CLMESSAGEOBSERVER_H
#define CLMESSAGEOBSERVER_H

#include "CLStatus.h"
#include "CLMessage.h"

class CLMessageLoopManager;

class CLMessageObserver{
    public:
	CLMessageObserver();
	virtual ~CLMessageObserver();

	virtual CLStatus Initialize(CLMessageLoopManager*,void* pContext) = 0;

    private:
	CLMessageObserver(const CLMessageObserver&);
	CLMessageObserver& operator=(const CLMessageObserver&);
};
#endif
