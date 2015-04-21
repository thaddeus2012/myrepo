#include "CLMessageLoopManager.h"
#include <iostream>
using std::cout;
using std::endl;

CLMessageLoopManager::CLMessageLoopManager(CLMessageObserver* pMessageObserver){
    if(pMessageObserver == 0)
	throw "In CLMessageLoopManager::CLMessageLoopManager(), pMessageObserver error";

    m_pMessageObserver = pMessageObserver;
}

CLMessageLoopManager::~CLMessageLoopManager(){
    delete m_pMessageObserver;
    cout<<"CLMessageLoopManager::~CLMessageLoopManager()"<<endl;
}

CLStatus CLMessageLoopManager::Register(unsigned long lMsgID, CallBackForMessageLoop pMsgProcessFunction){

    if(pMsgProcessFunction == NULL)
	return CLStatus(-1,0);
    m_MsgMappingTable[lMsgID] = pMsgProcessFunction;

    return CLStatus(0,0);
}

CLStatus CLMessageLoopManager::EnterMessageLoop(void* pContext){

    SLExecutiveInitialParameter* para = (SLExecutiveInitialParameter*)pContext;
    if(para == NULL || para->pNotifier == NULL)
	return CLStatus(-1,0);

    CLStatus s = Initialize();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Initialize error", 0);
	para->pNotifier->NotifyInitialFinished(false);
	return CLStatus(-1, 0);
    }

    CLStatus s1 = m_pMessageObserver->Initialize(this,para->pContext);
    if(!s1.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), m_pMessageObserver->Initialize error", 0);

	CLStatus s2 = Uninitialize();
	if(!s2.IsSuccess())
	    CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Uninitialize() error", 0);

	para->pNotifier->NotifyInitialFinished(false);
	return CLStatus(-1,0);
    }

    para->pNotifier->NotifyInitialFinished(true);

    while(true){
	CLMessage* pMsg = WaitForMessage();
	if(pMsg == 0){
	    CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), pMsg == 0", 0);
	    continue;
	}

	CLStatus s = DispatchMessage(pMsg);
	delete pMsg;

	if(s.m_clReturnCode == QUIT_MSG_LOOP)
	    break;
    }

    CLStatus s2 = Uninitialize();
    if(!s2.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Uninitialize() error", 0);
	return CLStatus(-1, 0);
    }
    
    return CLStatus(0, 0);
}

CLStatus CLMessageLoopManager::DispatchMessage(CLMessage* pMsg){
    map<unsigned long, CallBackForMessageLoop>::iterator it;
    it = m_MsgMappingTable.find(pMsg->m_clMsgID);

    if(it == m_MsgMappingTable.end()){
	CLLogger::WriteLogMsg("In CLMessageLoopManager::DispatchMessage(), it->second == 0", 0);
	return CLStatus(-1, 0);
    }

    CallBackForMessageLoop pFunction = it->second;
    return (m_pMessageObserver->*pFunction)(pMsg);
}
