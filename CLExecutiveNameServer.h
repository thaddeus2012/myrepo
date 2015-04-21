#ifndef CLEXECUTIVENAMESERVER_H
#define CLEXECUTIVENAMESERVER_H

#include <string>
#include <map>
#include "CLStatus.h"
#include "CLMutex.h"
#include "CLMessage.h"
//#include <pthread.h>
//#include <map>
//#include <string>
//#include "CLStatus.h"
//#include "CLMutex.h"

using std::map;
using std::string;

class CLExecutiveCommunication;

struct SLExecutiveCommunicationPtrCount{
    CLExecutiveCommunication *pExecutiveCommunication;
    unsigned nCount;
};

class CLExecutiveNameServer{
    public:
	CLExecutiveNameServer();
	virtual ~CLExecutiveNameServer();

	static CLExecutiveNameServer* GetInstance();
	static CLStatus PostExecutiveMessage(const char*, CLMessage*);

	CLStatus Register(const char*,CLExecutiveCommunication*);
	CLExecutiveCommunication* GetCommunicationPtr(const char*);
	CLStatus ReleaseCommunicationPtr(const char*);

    private:
	CLExecutiveNameServer(const CLExecutiveNameServer&);
	CLExecutiveNameServer& operator=(const CLExecutiveNameServer&);

	static pthread_mutex_t* InitializeMutex();

    private:
	static CLExecutiveNameServer* m_pNameServer;
	static pthread_mutex_t* m_pMutex;

	map<string,SLExecutiveCommunicationPtrCount*> m_NameTable;
	CLMutex m_MutexForNameTable;
};
#endif
