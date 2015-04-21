#include "CLThreadCommunicationBySTLQueue.h"
#include <iostream>
using std::cout;
using std::endl;

CLThreadCommunicationBySTLQueue::CLThreadCommunicationBySTLQueue(CLMessageQueueBySTLQueue* pMsgQueue){
    if(pMsgQueue == NULL)
	throw "In CLThreadCommunicationBySTLqueue::CLThreadCommunicationBySTLqueue(), pMsgQueue error";

    m_pMsgQueue = pMsgQueue;
}

CLThreadCommunicationBySTLQueue::~CLThreadCommunicationBySTLQueue(){
    cout<<"CLThreadCommunicationBySTLQueue::~CLThreadCommunicationBySTLQueue()"<<endl;
    delete m_pMsgQueue;
}

CLStatus CLThreadCommunicationBySTLQueue::PostExecutiveMessage(CLMessage* pMsg){
    if(pMsg == NULL)
	return CLStatus(-1,0);

    return m_pMsgQueue->PushMessage(pMsg);
}
