#ifndef CLMSGLOOPMANAGERFORSTLQUEUE_H
#define CLMSGLOOPMANAGERFORSTLQUEUE_H

#include "CLMessageLoopManager.h"
#include "CLMessageQueueBySTLQueue.h"
#include "CLMessageObserver.h"

class CLMsgLoopManagerForSTLQueue: public CLMessageLoopManager{
    public:
	CLMsgLoopManagerForSTLQueue(CLMessageObserver*,CLMessageQueueBySTLQueue*);
	virtual ~CLMsgLoopManagerForSTLQueue();

    protected:
	virtual CLStatus Initialize();
	virtual CLStatus Uninitialize();

	virtual CLMessage* WaitForMessage();

    private:
	CLMessageQueueBySTLQueue* m_pMsgQueue;
};
#endif
