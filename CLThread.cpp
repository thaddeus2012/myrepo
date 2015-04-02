#include <iostream>
#include "CLThread.h"
#include "CLLogger.h"

using std::cout;
using std::endl;

CLThread::CLThread(CLExcutiveFunctionProvider* pExcutiveFunctionProvider):CLExcutive(pExcutiveFunctionProvider){
}

CLThread::~CLThread(){}

CLStatus CLThread::Run(void* pContext){
    m_pContext = pContext;

    int r = pthread_create(&m_ThreadID,0,StartFunctionOfThread,this);
    if(r != 0){
	CLLogger::WriteLogMsg("In CLThread::Run(),pthread_create error",r);
	return CLStatus(-1,r);
    }

    return CLStatus(0,0);
}

CLStatus CLThread::WaitForDeath(){
    int r = pthread_join(m_ThreadID,0);
    if(r != 0){
	CLLogger::WriteLogMsg("In CLThread::WaitForDeath(),pthread_join error",r);
	return CLStatus(-1,r);
    }

    return CLStatus(0,0);
}

void* CLThread::StartFunctionOfThread(void* pThis){
    CLThread* pThreadThis = (CLThread*)pThis;

    CLStatus s = pThreadThis->m_pExcutiveFunctionProvider->RunExcutiveFunction(pThreadThis->m_pContext);

    return (void*)s.m_clReturnCode;
}

