#include "CLCriticalSection.h"

CLCriticalSection::CLCriticalSection(CLMutex* pMutex){
    if(pMutex == NULL){
	CLLogger::WriteLogMsg("In CLCriticalSection::CLCriticalSection(),pMutex is NULL",0);
	throw "In CLCriticalSection::CLCriticalSection(),pMutex == NULL";
    }

    m_pMutex = pMutex;

    CLStatus s = m_pMutex->Lock();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLCriticalSection::CLCriticalSection(),m_pMutex->Lock() error",0);
	throw "In CLCriticalSection::CLCriticalSection(),m_pMutex->Lock() error";
    }
}

CLCriticalSection::~CLCriticalSection(){
    CLStatus s = m_pMutex->UnLock();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLCriticalSection::~CLCriticalSection(),m_pMutex->UnLock() error",0);
	throw "In CLCriticalSection::~CLCriticalSection(),m_pMutex->UnLock() error";
    }
}
