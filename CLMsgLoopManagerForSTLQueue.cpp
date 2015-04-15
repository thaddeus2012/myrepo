#include "CLMsgLoopManagerForSTLQueue.h"
#include <iostream>
using std::cout;
using std::endl;

CLMsgLoopManagerForSTLQueue::CLMsgLoopManagerForSTLQueue(CLMessageObserver* pMessageObserver,CLMessageQueueBySTLQueue* pMsgQueue):CLMessageLoopManager(pMessageObserver){
    if(pMsgQueue == NULL)
	throw "In CLMsgLoopManagerForSTLqueue::CLMsgLoopManagerForSTLqueue(), pMsgQueue error";

    m_pMsgQueue = pMsgQueue;
}

CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue(){
    delete m_pMsgQueue;
    cout<<"CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue()"<<endl;
}

CLStatus CLMsgLoopManagerForSTLQueue::Initialize(){
    return CLStatus(0,0);
}

CLStatus CLMsgLoopManagerForSTLQueue::Uninitialize(){
    return CLStatus(0,0);
}

CLMessage* CLMsgLoopManagerForSTLQueue::WaitForMessage(){
    return m_pMsgQueue->GetMessage();
}
