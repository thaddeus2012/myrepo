#include "CLMessageLoopManager.h"
#include <iostream>
using std::cout;
using std::endl;

CLMessageLoopManager::CLMessageLoopManager(){}

CLMessageLoopManager::~CLMessageLoopManager(){
    cout<<"CLMessageLoopManager::~CLMessageLoopManager()"<<endl;
}

CLStatus CLMessageLoopManager::EnterMessageLoop(void* pContext){
    CLStatus s = Initialize();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLMessageLoopManager::EnterMessageLoop(), Initialize error", 0);
	return CLStatus(-1, 0);
    }

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
