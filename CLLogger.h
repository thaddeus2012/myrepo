#ifndef CLLOGGER_H
#define CLLOGGER_H

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

    private:
	int m_fd;
	static CLLogger* m_pLog;
	char* m_pLogBuffer;
	unsigned m_nUsedBytesForBuffer;
	bool m_bFlagForProcessExit;
};
#endif
