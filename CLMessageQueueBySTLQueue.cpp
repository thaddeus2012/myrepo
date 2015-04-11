#include "CLMessageQueueBySTLQueue.h"
#include "CLCriticalSection.h"
#include "CLLogger.h"
#include <iostream>
using std::cout;
using std::endl;

CLMessageQueueBySTLQueue::CLMessageQueueBySTLQueue():m_Event(true){}

CLMessageQueueBySTLQueue::~CLMessageQueueBySTLQueue(){
    while(true){ //清除在发送了Quit消息后可能又被送入队列的未处理消息
	CLMessage* pMsg = Pop();
	if(pMsg == NULL)
	    break;
	delete pMsg;
    }
    cout<<"CLMessageQueueBySTLQueue::~CLMessageQueueBySTLQueue()"<<endl;
}

CLStatus CLMessageQueueBySTLQueue::PushMessage(CLMessage* pMessage){
    if(pMessage == NULL)
	return CLStatus(-1,0);

    CLStatus s = Push(pMessage);
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(), Push error", 0);
	return CLStatus(-1, 0);
    }

    CLStatus s2 = m_Event.Set();
    if(!s2.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(), m_Event.Set error", 0);
	return CLStatus(-1, 0);
    }
    
    return CLStatus(0, 0);
}

CLMessage* CLMessageQueueBySTLQueue::GetMessage(){
    CLStatus s = m_Event.Wait();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageQueue::GetMessage(), m_Event.Wait error", 0);
	return NULL;
    }
    
    return Pop();
}

CLStatus CLMessageQueueBySTLQueue::Push(CLMessage* pMessage){
    try{
	CLCriticalSection cs(&m_Mutex);

	m_MessageQueue.push(pMessage);
    }catch(const char* str){
	CLLogger::WriteLogMsg("In CLMessageQueue::Push(), exception arise", 0);
	return CLStatus(-1, 0);	
    }

    return CLStatus(0,0);
}

CLMessage* CLMessageQueueBySTLQueue::Pop(){
    try{
	CLCriticalSection cs(&m_Mutex);

	if(m_MessageQueue.empty())
	    return NULL;

	CLMessage* p = m_MessageQueue.front();
	m_MessageQueue.pop();
	return p;
    }catch(const char* str){
	CLLogger::WriteLogMsg("In CLMessageQueue::Pop(), exception arise", 0);
	return NULL;
    }

}
