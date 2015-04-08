#ifndef CLLOGGER_H
#define CLLOGGER_H

#include <pthread.h>
#include "CLStatus.h"

class CLLogger{
    public:
	static CLLogger* GetInstance();
	static CLStatus WriteLogMsg(const char* pstrMsg,long lErrorCode);
	CLStatus WriteLog(const char* pstrMsg, long lErrorCode);
	CLStatus Flush();

    private:
	CLLogger(const CLLogger&);
	CLLogger& operator=(const CLLogger&);

	CLLogger();
	~CLLogger();

	static void OnProcessExit();
	
	static pthread_mutex_t* InitializeMutex();
	CLStatus WriteMsgAndErrcode(const char* pstrMsg,const char* pstrErrcode);

    private:
	int m_fd;
	char* m_pLogBuffer;
	unsigned m_nUsedBytesForBuffer;
	bool m_bFlagForProcessExit;
	pthread_mutex_t* m_pMutexForWritingLog;
	static pthread_mutex_t* m_pMutexForCreatingLogger;
	static CLLogger* m_pLog;
};
#endif
