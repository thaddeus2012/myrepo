#ifndef CLTHREADCOMMUNICATIONBYSTLQUEUE_H
#define CLTHREADCOMMUNICATIONBYSTLQUEUE_H

#include "CLStatus.h"
#include "CLExecutiveCommunication.h"
#include "CLMessageQueueBySTLQueue.h"

class CLThreadCommunicationBySTLQueue: public CLExecutiveCommunication{
    public:
	CLThreadCommunicationBySTLQueue(CLMessageQueueBySTLQueue*);
	virtual ~CLThreadCommunicationBySTLQueue();

	virtual CLStatus PostExecutiveMessage(CLMessage*);

    private:
	CLThreadCommunicationBySTLQueue(const CLThreadCommunicationBySTLQueue&);
	CLThreadCommunicationBySTLQueue& operator=(const CLThreadCommunicationBySTLQueue&);

    private:
	CLMessageQueueBySTLQueue* m_pMsgQueue;
};
#endif
