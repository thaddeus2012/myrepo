#ifndef CLMSGLOOPMANAGERFORSTLQUEUE_H
#define CLMSGLOOPMANAGERFORSTLQUEUE_H

#include "CLMessageLoopManager.h"
#include "CLMessageQueueBySTLQueue.h"

class CLMsgLoopManagerForSTLQueue: public CLMessageLoopManager{
    public:
	CLMsgLoopManagerForSTLQueue(CLMessageQueueBySTLQueue*);
	virtual ~CLMsgLoopManagerForSTLQueue();

    protected:
	virtual CLStatus Initialize();
	virtual CLStatus Uninitialize();

	virtual CLMessage* WaitForMessage();

    private:
	CLMessageQueueBySTLQueue* m_pMsgQueue;
};
#endif
