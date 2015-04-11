#ifndef CLMESSAGEQUEUEBYSTLQUEUE_H
#define CLMESSAGEQUEUEBYSTLQUEUE_H

#include "CLStatus.h"
#include "CLMessage.h"
#include "CLEvent.h"
#include <queue>
using std::queue;

class CLMessageQueueBySTLQueue{
    public:
	CLMessageQueueBySTLQueue();
	virtual ~CLMessageQueueBySTLQueue();

    public:
	CLStatus PushMessage(CLMessage* pMessage);
	CLMessage* GetMessage();

    private:
	CLStatus Push(CLMessage* pMessage);
	CLMessage* Pop();

    private:
	CLMessageQueueBySTLQueue(const CLMessageQueueBySTLQueue&);
	CLMessageQueueBySTLQueue& operator=(const CLMessageQueueBySTLQueue&);

    private:
	queue<CLMessage*> m_MessageQueue;
	CLMutex m_Mutex;
	CLEvent m_Event;
};

#endif
