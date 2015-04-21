#include <cstring>
#include "CLExecutiveNameServer.h"
#include "CLCriticalSection.h"
#include "CLExecutiveCommunication.h"
//#include "CLExecutiveNameServer.h"
//#include "CLCriticalSection.h"
//#include "CLLogger.h"
//#include "CLExecutiveCommunication.h"
//#include "CLMessage.h"

CLExecutiveNameServer* CLExecutiveNameServer::m_pNameServer = 0;
pthread_mutex_t* CLExecutiveNameServer::m_pMutex = CLExecutiveNameServer::InitializeMutex();

CLExecutiveNameServer::CLExecutiveNameServer(){}

CLExecutiveNameServer::~CLExecutiveNameServer(){}

CLStatus CLExecutiveNameServer::Register(const char* strExecutiveName,CLExecutiveCommunication* pExecutiveCommunication){

    if(pExecutiveCommunication == NULL)
	return CLStatus(-1,0);

    if(strExecutiveName == NULL || strlen(strExecutiveName) == 0){
	delete pExecutiveCommunication;
	return CLStatus(-1,0);
    }

    CLCriticalSection cs(&m_MutexForNameTable);

    map<string,SLExecutiveCommunicationPtrCount*>::iterator it = m_NameTable.find(strExecutiveName);
    if(it != m_NameTable.end()){
	delete pExecutiveCommunication;
	CLLogger::WriteLogMsg("In CLExecutiveNameServer::Register(), m_NameTable.find error", 0);
	return CLStatus(-1,0);
    }
    
    SLExecutiveCommunicationPtrCount* p = new SLExecutiveCommunicationPtrCount();
    p->pExecutiveCommunication = pExecutiveCommunication;
    p->nCount = 1;

    m_NameTable[strExecutiveName] = p;
    return CLStatus(0,0);
}

CLExecutiveCommunication* CLExecutiveNameServer::GetCommunicationPtr(const char* strExecutiveName){
    if(strExecutiveName == NULL || strlen(strExecutiveName) == 0)
	return NULL;

    CLCriticalSection cs(&m_MutexForNameTable);

    map<string,SLExecutiveCommunicationPtrCount*>::iterator it = m_NameTable.find(strExecutiveName);
    if(it == m_NameTable.end()){
	CLLogger::WriteLogMsg("In CLExecutiveNameServer::GetCommunicationPtr(), m_NameTable.find error", 0);
	return NULL;
    }

    it->second->nCount++;
    return it->second->pExecutiveCommunication;
}

CLStatus CLExecutiveNameServer::ReleaseCommunicationPtr(const char* strExecutiveName){
    if(strExecutiveName == NULL || strlen(strExecutiveName) == 0)
	return CLStatus(-1,0);

    CLExecutiveCommunication* pTmp = NULL;
    {
	CLCriticalSection cs(&m_MutexForNameTable);

    	map<string,SLExecutiveCommunicationPtrCount*>::iterator it = m_NameTable.find(strExecutiveName);
    	if(it == m_NameTable.end()){
    	    CLLogger::WriteLogMsg("In CLExecutiveNameServer::ReleaseCommunicationPtr(), m_NameTable.find error", 0);
    	    return CLStatus(-1,0);
    	}

    	it->second->nCount--;
    	if(it->second->nCount == 0){
    	    pTmp = it->second->pExecutiveCommunication;
    	    delete it->second;
    	    m_NameTable.erase(it);
    	}
    }

    //在加锁区域外删除Communication对象，
    //因为该对象的析构函数会删除消息队列，
    //而消息队列的析构会清除还在队列中的消息(如果有的话)，
    //如果在这些消息对象的析构中又调用了NameServer的方法
    //对同一把锁加锁就会产生死锁
    if(pTmp != NULL) 
	delete pTmp;

    return CLStatus(0,0);
}

pthread_mutex_t* CLExecutiveNameServer::InitializeMutex(){

    pthread_mutex_t* p = new pthread_mutex_t;

    int r = pthread_mutex_init(p, 0);
    if(r != 0)
    {
	delete p;
	return NULL;
    }
    
    return p;
}

CLExecutiveNameServer* CLExecutiveNameServer::GetInstance()
{
    if(m_pNameServer == NULL)
    {
	if(m_pMutex == NULL)
	    return NULL;
    	
	int r = pthread_mutex_lock(m_pMutex);
	if(r != 0)
	    return NULL;
    	
	if(m_pNameServer == NULL){
	    m_pNameServer = new CLExecutiveNameServer;
	}
    
	r = pthread_mutex_unlock(m_pMutex);
	if(r != 0)
	    return NULL;
    }
    
    return m_pNameServer;
}

CLStatus CLExecutiveNameServer::PostExecutiveMessage(const char* pstrExecutiveName, CLMessage* pMessage){

    if(pMessage == 0)
	return CLStatus(-1, 0);

    if((pstrExecutiveName == 0) || (strlen(pstrExecutiveName) == 0)){
	delete pMessage;
	return CLStatus(-1, 0);
    }

    CLExecutiveNameServer* pNameServer = CLExecutiveNameServer::GetInstance();
    if(pNameServer == NULL){
	CLLogger::WriteLogMsg("In CLExecutiveNameServer::PostExecutiveMessage(), CLExecutiveNameServer::GetInstance error", 0);
	delete pMessage;
	return CLStatus(-1,0);
    }

    CLExecutiveCommunication* pComm = pNameServer->GetCommunicationPtr(pstrExecutiveName);
    if(pComm == NULL){
	CLLogger::WriteLogMsg("In CLExecutiveNameServer::PostExecutiveMessage(), pNameServer->GetCommunicationPtr error", 0);
	delete pMessage;
	return CLStatus(-1,0);
    }

    CLStatus s = pComm->PostExecutiveMessage(pMessage);
    if(!s.IsSuccess()){
	 CLLogger::WriteLogMsg("In CLExecutiveNameServer::PostExecutiveMessage(), pComm->PostExecutiveMessage error", 0);

	 CLStatus s = pNameServer->ReleaseCommunicationPtr(pstrExecutiveName);
	 if(!s.IsSuccess())
	     CLLogger::WriteLogMsg("In CLExecutiveNameServer::PostExecutiveMessage(), pNameServer->ReleaseCommunicationPtr error", 0);
	 return CLStatus(-1,0);
    }

    CLStatus s2 = pNameServer->ReleaseCommunicationPtr(pstrExecutiveName);
    if(!s2.IsSuccess())
	CLLogger::WriteLogMsg("In CLExecutiveNameServer::PostExecutiveMessage(), pNameServer->ReleaseCommunicationPtr error", 0);

    return CLStatus(0, 0);
}
