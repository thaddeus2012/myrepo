#include "CLExecutiveFunctionForMsgLoop.h"
#include <iostream>
using std::cout;
using std::endl;

CLExecutiveFunctionForMsgLoop::CLExecutiveFunctionForMsgLoop(CLMessageLoopManager* pMsgLoopManager){
    if(pMsgLoopManager == 0)
	throw "In CLExecutiveFunctionForMsgLoop::CLExecutiveFunctionForMsgLoop(), pMsgLoopManager error";
    m_pMsgLoopManager = pMsgLoopManager;
}

CLExecutiveFunctionForMsgLoop::~CLExecutiveFunctionForMsgLoop(){
    cout<<"CLExecutiveFunctionForMsgLoop::~CLExecutiveFunctionForMsgLoop()"<<endl;
    if(m_pMsgLoopManager != NULL)
	delete m_pMsgLoopManager;
}

CLStatus CLExecutiveFunctionForMsgLoop::RunExecutiveFunction(void* pContext){
    return m_pMsgLoopManager->EnterMessageLoop(pContext);
}
