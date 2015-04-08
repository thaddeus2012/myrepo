#include <iostream>
#include "CLThread.h"
#include "CLLogger.h"
#include "CLCoordinator.h"

using std::cout;
using std::endl;

CLThread::CLThread(CLCoordinator* pCoordinator):CLExecutive(pCoordinator),m_bThreadCreated(false),m_bWaitForDeath(false){
}

CLThread::CLThread(CLCoordinator* pCoordinator,bool bWaitForDeath):CLExecutive(pCoordinator),m_bThreadCreated(false),m_bWaitForDeath(bWaitForDeath){
}

CLThread::~CLThread(){}

CLStatus CLThread::Run(){
    if(m_bThreadCreated)
	return CLStatus(-1,0);

    int r = pthread_create(&m_ThreadID,0,StartFunctionOfThread,this);
    if(r != 0){
	CLLogger::WriteLogMsg("In CLThread::Run(),pthread_create error",r);
	delete this;
	return CLStatus(-1,r);
    }

    m_bThreadCreated = true;

    if(!m_bWaitForDeath){
	int r = pthread_detach(m_ThreadID);
	if(r != 0)
	    CLLogger::WriteLogMsg("In CLThread::Run(), pthread_detach error", r);
    }

    CLStatus s = m_EventForWaitingForNewThread.Wait();
    if(!s.IsSuccess())
	CLLogger::WriteLogMsg("In CLThread::Run(), m_EventForWaitingForNewThread.Wait error",s.m_clErrorCode);

    CLStatus s1 = m_EventForWaitingForOldThread.Set();
    if(!s1.IsSuccess())
	CLLogger::WriteLogMsg("In CLThread::Run(), m_EventForWaitingForOldThread.Set error",s1.m_clErrorCode);

    return CLStatus(0,0);
}

CLStatus CLThread::WaitForDeath(){
    if(m_bWaitForDeath == false)
	return CLStatus(-1,0);

    if(m_bThreadCreated == false)
	return CLStatus(-1,0);

    int r = pthread_join(m_ThreadID,0);
    if(r != 0){
	CLLogger::WriteLogMsg("In CLThread::WaitForDeath(),pthread_join error",r);
	return CLStatus(-1,r);
    }

    delete this;

    return CLStatus(0,0);
}

void* CLThread::StartFunctionOfThread(void* pThis){
    CLThread* pThreadThis = (CLThread*)pThis;

    CLStatus s = pThreadThis->m_EventForWaitingForNewThread.Set();
    if(!s.IsSuccess())
	CLLogger::WriteLogMsg("In CLThread::StartFunctionOfThread(), m_EventForWaitingForNewThread.Set error",s.m_clErrorCode);

    CLStatus s1 = pThreadThis->m_EventForWaitingForOldThread.Wait();
    if(!s1.IsSuccess())
	CLLogger::WriteLogMsg("In CLThread::StartFunctionOfThread(), m_EventForWaitingForOldThread.Wait error",s1.m_clErrorCode);

    CLStatus s2 = pThreadThis->m_pCoordinator->ReturnControlRights();

    if(!pThreadThis->m_bWaitForDeath){
	delete pThreadThis;
    }

    return (void*)s.m_clReturnCode;
}

