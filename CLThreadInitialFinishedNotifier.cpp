#include "CLThreadInitialFinishedNotifier.h"
#include <iostream>
using std::cout;
using std::endl;

CLThreadInitialFinishedNotifier::CLThreadInitialFinishedNotifier(CLEvent* pEvent){
    m_pEvent = pEvent;
    m_bSuccess = false;
}

CLThreadInitialFinishedNotifier::~CLThreadInitialFinishedNotifier(){
    cout<<"CLThreadInitialFinishedNotifier::~CLThreadInitialFinishedNotifier()"<<endl;
}

CLStatus CLThreadInitialFinishedNotifier::NotifyInitialFinished(bool bInitialSuccess){
    m_bSuccess = bInitialSuccess;

    if(m_pEvent == NULL)
	return CLStatus(-1,0);

    CLStatus s = m_pEvent->Set();
    if(!s.IsSuccess()){
	CLLogger::WriteLogMsg("In CLThreadInitialFinishedNotifier::NotifyInitialFinished(), m_pEvent->Set error", 0);
	return CLStatus(-1, 0);
    }

    return CLStatus(0,0);
}

bool CLThreadInitialFinishedNotifier::IsInitialSuccess(){
    return m_bSuccess;
}
