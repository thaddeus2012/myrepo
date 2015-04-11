#ifndef CLMESSAGELOOPMANAGER_H
#define CLMESSAGELOOPMANAGER_H

#include "CLStatus.h"
#include "CLLogger.h"
#include "CLMessage.h"

#define QUIT_MSG_LOOP 1

class CLMessageLoopManager{
    public:
	CLMessageLoopManager();
	virtual ~CLMessageLoopManager();

	virtual CLStatus EnterMessageLoop(void* pContext);

    protected:
	virtual CLStatus Initialize() = 0;
	virtual CLStatus Uninitialize() = 0;

	virtual CLMessage* WaitForMessage() = 0;
	virtual CLStatus DispatchMessage(CLMessage* pMessage) = 0;

    private:
	CLMessageLoopManager(const CLMessageLoopManager&);
	CLMessageLoopManager& operator=(const CLMessageLoopManager&);
};
#endif
