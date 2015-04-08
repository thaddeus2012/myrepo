#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "CLLogger.h"

#define LOG_FILE_NAME "CLLogger.txt"
#define MAX_SIZE 256
#define BUFFER_SIZE 4096

CLLogger* CLLogger::m_pLog = 0;

pthread_mutex_t* CLLogger::m_pMutexForCreatingLogger = CLLogger::InitializeMutex();

pthread_mutex_t* CLLogger::InitializeMutex(){
    pthread_mutex_t* p = new pthread_mutex_t;

    if(pthread_mutex_init(p,0) != 0){
	delete p;
	return 0;
    }

    return p;
}

CLLogger::CLLogger():m_fd(-1),
    m_nUsedBytesForBuffer(0),
    m_bFlagForProcessExit(false)
{
    m_fd = open(LOG_FILE_NAME,O_RDWR|O_CREAT|O_APPEND,
	        S_IRUSR|S_IWUSR);
    if(m_fd == -1)
	throw "In CLLogger::CLLogger(), open error";

    m_pLogBuffer = new char[BUFFER_SIZE];

    m_pMutexForWritingLog = new pthread_mutex_t;
    if(pthread_mutex_init(m_pMutexForWritingLog,0) != 0){
	delete m_pMutexForWritingLog;
	delete [] m_pLogBuffer;
	close(m_fd);

	throw "In CLLogger::CLLogger(), pthread_mutex_init error";
    }
}

CLLogger::~CLLogger(){
    if(m_fd != -1)
	close(m_fd);
}

CLLogger* CLLogger::GetInstance(){
    if(m_pLog != NULL)
	return m_pLog;

    if(m_pMutexForCreatingLogger == 0)
	return NULL;

    if(pthread_mutex_lock(m_pMutexForCreatingLogger) != 0)
	return NULL;

    if(m_pLog == NULL){

	try{
	    m_pLog = new CLLogger();
	}catch(const char*){
	    pthread_mutex_unlock(m_pMutexForCreatingLogger);
	    return NULL;
	}
        if(pthread_mutex_unlock(m_pMutexForCreatingLogger) != 0)
	    return NULL;

	if(atexit(CLLogger::OnProcessExit) != 0){

	    if(m_pLog != NULL){
		m_pLog->m_bFlagForProcessExit = true;
		m_pLog->WriteLog("In CLLogger::GetInstance(),atexit error",errno);
		//m_pLog->Flush();
	    }
	    if(pthread_mutex_unlock(m_pMutexForCreatingLogger) != 0)
		return 0;
	}else
	    if(pthread_mutex_unlock(m_pMutexForCreatingLogger) != 0)
		return 0;
	return m_pLog;
    }
    if(pthread_mutex_unlock(m_pMutexForCreatingLogger) != 0)
	return 0;

    return m_pLog;
}

CLStatus CLLogger::WriteLogMsg(const char* pstrMsg,long lErrorCode){
    CLLogger* pLogger = GetInstance();
    if(pLogger == NULL)
	return CLStatus(-1,0);

    CLStatus s = pLogger->WriteLog(pstrMsg,lErrorCode);
    if(!s.IsSuccess())
	return CLStatus(s);
    return CLStatus(0,0);
}

CLStatus CLLogger::Flush(){
    if(pthread_mutex_lock(m_pMutexForWritingLog) != 0)
	return CLStatus(-1,0);

    try{
        if(m_fd == -1)
            throw CLStatus(-1,0);

        if(m_pLogBuffer == NULL)
            throw CLStatus(-1,0);

        if(m_nUsedBytesForBuffer == 0)
            throw CLStatus(0,0);

        if(write(m_fd,m_pLogBuffer,m_nUsedBytesForBuffer) == -1)
            throw CLStatus(-1,errno);

        m_nUsedBytesForBuffer = 0;

        throw CLStatus(0,0);

    }catch(CLStatus &s){
	if(pthread_mutex_unlock(m_pMutexForWritingLog) != 0)
	    return CLStatus(-1,0);

	return s;
    }
}

CLStatus CLLogger::WriteMsgAndErrcode(const char* pstrMsg,const char* pstrErrcode){
    if(write(m_fd,pstrMsg,strlen(pstrMsg)) == -1)
	return CLStatus(-1,errno);

    if(write(m_fd,pstrErrcode,strlen(pstrErrcode)) == -1)
	return CLStatus(-1,errno);

    return CLStatus(0,0);
}

CLStatus CLLogger::WriteLog(const char* pstrMsg,long lErrorCode){
    if(pstrMsg == NULL)
	return CLStatus(-1,0);

    if(strlen(pstrMsg) == 0)
	return CLStatus(-1,0);

    if(m_pLogBuffer == NULL)
	return CLStatus(-1,0);

    if(m_fd == -1)
	return CLStatus(-1,0);

    unsigned len_strmsg = strlen(pstrMsg);

    char buf[MAX_SIZE];
    snprintf(buf,MAX_SIZE," Error Code: %ld\r\n",lErrorCode);
    unsigned len_code = strlen(buf);

    unsigned total_len = len_strmsg + len_code;

    if(pthread_mutex_lock(m_pMutexForWritingLog) != 0)
	return CLStatus(-1,0);

    try{
        if(total_len > BUFFER_SIZE || m_bFlagForProcessExit){
            //CLStatus s = Flush();
            //if(!s.IsSuccess())
            //    return CLStatus(s);

            //ssize_t r = write(m_fd,pstrMsg,len_strmsg);
            //if(r == -1)
            //    return CLStatus(-1,errno);

            //r = write(m_fd,buf,len_code);
            //if(r == -1)
            //    return CLStatus(-1,errno);
            //return CLStatus(0,0);
	    throw WriteMsgAndErrcode(pstrMsg,buf);
        }

        unsigned nLeftRoom = BUFFER_SIZE - m_nUsedBytesForBuffer;
        if(total_len > nLeftRoom){
            //CLStatus s = Flush();
            //if(!s.IsSuccess())
            //    throw CLStatus(s);
	    if(write(m_fd,m_pLogBuffer,m_nUsedBytesForBuffer) == -1)
		throw CLStatus(-1,0);

	    m_nUsedBytesForBuffer = 0;
        }

        memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, pstrMsg, len_strmsg);

        m_nUsedBytesForBuffer += len_strmsg;

        memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);

        m_nUsedBytesForBuffer += len_code;

        throw CLStatus(0,0);

    }catch(CLStatus &s){
	if(pthread_mutex_unlock(m_pMutexForWritingLog) != 0)
	    return CLStatus(-1,0);

	return s;
    }
}

void CLLogger::OnProcessExit(){
    CLLogger* pLogger = GetInstance();
    if(pLogger){
	pthread_mutex_lock(pLogger->m_pMutexForWritingLog);
	pLogger->m_bFlagForProcessExit = true;
	pthread_mutex_unlock(pLogger->m_pMutexForWritingLog);
	pLogger->Flush();
    }
}
