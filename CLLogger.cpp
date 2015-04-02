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

CLLogger::CLLogger():m_fd(-1),
    m_nUsedBytesForBuffer(0),
    m_bFlagForProcessExit(false)
{
    m_fd = open(LOG_FILE_NAME,O_RDWR|O_CREAT|O_APPEND,
	        S_IRUSR|S_IWUSR);

    m_pLogBuffer = new char[BUFFER_SIZE];
}

CLLogger::~CLLogger(){
    if(m_fd != -1)
	close(m_fd);
}

CLLogger* CLLogger::GetInstance(){
    if(m_pLog == NULL){

	m_pLog = new CLLogger();

	if(atexit(CLLogger::OnProcessExit) != 0){
	    if(m_pLog != NULL){
		m_pLog->WriteLog("In CLLogger::GetInstance(),atexit error",errno);
		m_pLog->Flush();
	    }
	}
    }
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
    if(m_fd == -1)
	return CLStatus(-1,0);

    if(m_pLogBuffer == NULL)
	return CLStatus(-1,0);

    if(m_nUsedBytesForBuffer == 0)
	return CLStatus(0,0);

    ssize_t r = write(m_fd,m_pLogBuffer,m_nUsedBytesForBuffer);
    if(r == -1)
	return CLStatus(-1,errno);

    m_nUsedBytesForBuffer = 0;
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

    unsigned nLeftRoom = BUFFER_SIZE - m_nUsedBytesForBuffer;

    unsigned len_strmsg = strlen(pstrMsg);

    char buf[MAX_SIZE];
    snprintf(buf,MAX_SIZE," Error Code: %ld\r\n",lErrorCode);
    unsigned len_code = strlen(buf);

    unsigned total_len = len_strmsg + len_code;

    if(total_len > BUFFER_SIZE || m_bFlagForProcessExit){
	CLStatus s = Flush();
	if(!s.IsSuccess())
	    return CLStatus(s);

	ssize_t r = write(m_fd,pstrMsg,len_strmsg);
	if(r == -1)
	    return CLStatus(-1,errno);

	r = write(m_fd,buf,len_code);
	if(r == -1)
	    return CLStatus(-1,errno);
	return CLStatus(0,0);
    }

    if(total_len > nLeftRoom){
	CLStatus s = Flush();
	if(!s.IsSuccess())
	    return CLStatus(s);
    }

    memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, pstrMsg, len_strmsg);

    m_nUsedBytesForBuffer += len_strmsg;

    memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);

    m_nUsedBytesForBuffer += len_code;

    return CLStatus(0,0);
}

void CLLogger::OnProcessExit(){
    CLLogger* pLogger = GetInstance();
    if(pLogger){
	pLogger->Flush();
	pLogger->m_bFlagForProcessExit = true;
    }
}
