#include <iostream>
#include <cstring>
#include "CLCoordinatorForMsgLoop.h"
#include "CLExecutive.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLEvent.h"
#include "CLThreadInitialFinishedNotifier.h"
#include "CLMessageLoopManager.h"

#include "CLThread.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMsgLoopManagerForSTLQueue.h"

using std::cout;
using std::endl;

CLCoordinatorForMsgLoop::CLCoordinatorForMsgLoop(CLMessageObserver *pMsgObserver, const char *pstrThreadName){
    if(pMsgObserver == 0)
	throw "In CLThreadForMsgLoop::CLThreadForMsgLoop(), pMsgObserver error";
    
    if((pstrThreadName == 0) || (strlen(pstrThreadName) == 0))
	throw "In CLThreadForMsgLoop::CLThreadForMsgLoop(), pstrThreadName error";
    
    m_bWaitForDeath = false;

    m_pExecutive = NULL;
    m_pFunctionProvider = new CLExecutiveFunctionForMsgLoop(new CLMsgLoopManagerForSTLQueue(pMsgObserver, pstrThreadName));
}

CLCoordinatorForMsgLoop::CLCoordinatorForMsgLoop(CLMessageObserver *pMsgObserver, const char *pstrThreadName, bool bWaitForDeath){
    if(pMsgObserver == 0)
	throw "In CLThreadForMsgLoop::CLThreadForMsgLoop(), pMsgObserver error";
    
    if((pstrThreadName == 0) || (strlen(pstrThreadName) == 0))
	throw "In CLThreadForMsgLoop::CLThreadForMsgLoop(), pstrThreadName error";
    
    m_bWaitForDeath = bWaitForDeath;

    m_pExecutive = NULL;
    m_pFunctionProvider = new CLExecutiveFunctionForMsgLoop(new CLMsgLoopManagerForSTLQueue(pMsgObserver, pstrThreadName));
}

CLCoordinatorForMsgLoop::~CLCoordinatorForMsgLoop(){
    if((m_bWaitForDeath) && (m_pExecutive != 0)){
        CLStatus s = m_pExecutive->WaitForDeath();
        if(!s.IsSuccess())
            CLLogger::WriteLogMsg("In CLCoordinatorForMsgLoop::~CLCoordinatorForMsgLoop(), m_pExecutive->WaitForDeath error", 0);
    }
    cout<<"CLCoordinatorForMsgLoop::~CLCoordinatorForMsgLoop()"<<endl;
}

CLStatus CLCoordinatorForMsgLoop::Run(void* pContext){
    if(m_pExecutive == NULL){
	m_pExecutive = new CLThread(this,m_bWaitForDeath); //由于要传this指针所以没放到构造函数里
    }

    if(m_pExecutive == NULL || m_pFunctionProvider == NULL){
	return CLStatus(-1,0);
    }

    CLEvent event;
    CLThreadInitialFinishedNotifier notifier(&event);

    SLExecutiveInitialParameter para;
    para.pContext = pContext;
    para.pNotifier = &notifier;

    //m_pContext = pContext;
    m_pContext = (void*)&para;

    CLStatus s = m_pExecutive->Run();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLCoordinatorForMsgLoop::Run(), m_pExecutive->Run error", 0);
	return CLStatus(-1,0);
    }

    CLStatus s1 = event.Wait();
    if(!s1.IsSuccess())
	CLLogger::WriteLogMsg("In CLCoordinatorForMsgLoop::Run(), event.Wait error", 0);

    if(notifier.IsInitialSuccess())
	return CLStatus(0, 0);
    else
	return CLStatus(-1, 0);
}

CLStatus CLCoordinatorForMsgLoop::ReturnControlRights(){
    return m_pFunctionProvider->RunExecutiveFunction(m_pContext);
}

CLStatus CLCoordinatorForMsgLoop::WaitForDeath(){
    return m_pExecutive->WaitForDeath();
}
