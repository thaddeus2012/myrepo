#include "CLMsgLoopManagerForSTLQueue.h"
#include <iostream>
using std::cout;
using std::endl;

CLMsgLoopManagerForSTLQueue::CLMsgLoopManagerForSTLQueue(CLMessageQueueBySTLQueue* pMsgQueue){
    if(pMsgQueue == NULL)
	throw "In CLMsgLoopManagerForSTLqueue::CLMsgLoopManagerForSTLqueue(), pMsgQueue error";

    m_pMsgQueue = pMsgQueue;
}

CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue(){
    cout<<"CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue()"<<endl;
    delete m_pMsgQueue;
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
