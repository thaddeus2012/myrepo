#ifndef CLMSGLOOPMANAGERFORSTLQUEUE_H
#define CLMSGLOOPMANAGERFORSTLQUEUE_H

#include <string>
#include "CLMessageLoopManager.h"
#include "CLMessageQueueBySTLQueue.h"
#include "CLMessageObserver.h"

using std::string;

class CLMsgLoopManagerForSTLQueue: public CLMessageLoopManager{
    public:
	CLMsgLoopManagerForSTLQueue(CLMessageObserver*,const char*);
	virtual ~CLMsgLoopManagerForSTLQueue();

    protected:
	virtual CLStatus Initialize();
	virtual CLStatus Uninitialize();

	virtual CLMessage* WaitForMessage();

    private:
	CLMessageQueueBySTLQueue* m_pMsgQueue;
	string m_strThreadName;
};
#endif
