#include "CLMsgLoopManagerForSTLQueue.h"
#include "CLThreadCommunicationBySTLQueue.h"
#include "CLExecutiveNameServer.h"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;

CLMsgLoopManagerForSTLQueue::CLMsgLoopManagerForSTLQueue(CLMessageObserver* pMessageObserver,const char* pstrThreadName):CLMessageLoopManager(pMessageObserver){
    if(pstrThreadName == NULL || strlen(pstrThreadName) == 0)
	throw "In CLMsgLoopManagerForSTLqueue::CLMsgLoopManagerForSTLqueue(), pstrThreadName error";

    m_strThreadName = pstrThreadName;

    m_pMsgQueue = new CLMessageQueueBySTLQueue();
}

CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue(){
    //delete m_pMsgQueue; //消息队列改在通信类中删除了
    cout<<"CLMsgLoopManagerForSTLQueue::~CLMsgLoopManagerForSTLQueue()"<<endl;
}

CLStatus CLMsgLoopManagerForSTLQueue::Initialize(){
    CLExecutiveNameServer* pNameServer = CLExecutiveNameServer::GetInstance();
    if(pNameServer == NULL){
	delete m_pMsgQueue;
	m_pMsgQueue = NULL;
	CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), CLExecutiveNameServer::GetInstance error", 0);
	return CLStatus(-1,0);
    }

    CLStatus s = pNameServer->Register(m_strThreadName.c_str(),new CLThreadCommunicationBySTLQueue(m_pMsgQueue));
    if(!s.IsSuccess()){
	delete m_pMsgQueue;
	m_pMsgQueue = 0;
	CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize()    , pNameServer->Register error", 0);
	return CLStatus(-1,0);
    }

    return CLStatus(0,0);
}

CLStatus CLMsgLoopManagerForSTLQueue::Uninitialize(){

    CLExecutiveNameServer* pNameServer = CLExecutiveNameServer::GetInstance();
    if(pNameServer == NULL){
	delete m_pMsgQueue;
	m_pMsgQueue = NULL;
	CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Uninitialize(), CLExecutiveNameServer::GetInstance error", 0);
	return CLStatus(-1,0);
    }

    return pNameServer->ReleaseCommunicationPtr(m_strThreadName.c_str());
}

CLMessage* CLMsgLoopManagerForSTLQueue::WaitForMessage(){
    return m_pMsgQueue->GetMessage();
}
