#include "CLStatus.h"

CLStatus::CLStatus(long lReturnCode,long lErrorCode)
    :m_lReturnCode(lReturnCode),m_lErrorCode(lErrorCode),
     m_clReturnCode(m_lReturnCode),m_clErrorCode(m_lErrorCode)
{}

CLStatus::CLStatus(const CLStatus& s)
    :m_clReturnCode(m_lReturnCode),m_clErrorCode(m_lErrorCode)
{
    m_lReturnCode = s.m_lReturnCode;
    m_lErrorCode = s.m_lErrorCode;
}

CLStatus::~CLStatus(){}

bool CLStatus::IsSuccess(){
    if(m_lReturnCode >= 0)
	return true;
    else
	return false;
}
