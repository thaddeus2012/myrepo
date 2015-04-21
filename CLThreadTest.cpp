#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLRegularCoordinator.h"
#include "CLCoordinatorForMsgLoop.h"
#include "CLCriticalSection.h"
#include "CLConditionVariable.h"
#include "CLEvent.h"
#include "CLMessageQueueBySTLQueue.h"
#include "CLMsgLoopManagerForSTLQueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMsgLoopManagerForSTLQueue.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"

#define ADD_MSG 0
#define QUIT_MSG 1

using std::cout;
using std::endl;

class CLMyMsgProcessor;

class CLAddMessage: public CLMessage{
    public:
	friend class CLMyMsgProcessor;

	CLAddMessage(int op1,int op2):CLMessage(ADD_MSG){
	    m_Op1 = op1;
	    m_Op2 = op2;
	}
	virtual ~CLAddMessage(){
	    cout<<"CLAddMessage::~CLAddMessage(),Message ID = "<<m_lMsgID<<", "<<m_Op1<<" "<<m_Op2<<endl;
	}

    private:
	int m_Op1;
	int m_Op2;
};

class CLQuitMessage: public CLMessage{
    public:
	CLQuitMessage():CLMessage(QUIT_MSG){}

	virtual ~CLQuitMessage(){
	    cout<<"CLQuitMessage::~CLQuitMessage(),Message ID = "<<m_lMsgID<<endl;
	}
};

class CLMyMsgProcessor: public CLMessageObserver{
    public:
	CLMyMsgProcessor(){}
	virtual ~CLMyMsgProcessor(){
	    cout<<"CLMyMsgProcessor::~CLMyMsgProcessor()"<<endl;
	}

	virtual CLStatus Initialize(CLMessageLoopManager* pMessageLoop,void* pContext){
	    CLStatus s = pMessageLoop->Register(ADD_MSG,(CallBackForMessageLoop)(&CLMyMsgProcessor::On_AddMsg));
	    if(!s.IsSuccess())
		return s;
	    CLStatus s1 = pMessageLoop->Register(QUIT_MSG,(CallBackForMessageLoop)(&CLMyMsgProcessor::On_QuitMsg));
	    return s1;
	}

	CLStatus On_AddMsg(CLMessage* pMsg){
	    CLAddMessage* pAddMsg = (CLAddMessage*)pMsg;
	    cout<<pAddMsg->m_Op1 + pAddMsg->m_Op2<<endl;

	    return CLStatus(0,0);
	}

	CLStatus On_QuitMsg(CLMessage* pMsg){
	    cout<<"quit ..."<<endl;

	    return CLStatus(QUIT_MSG_LOOP,0);
	}
};

int main(){
    //只能在栈上分配CLCoordinatorForMsgLoop对象，堆上对象不能自行销毁，
    //因为CLThread对象的析构函数不再调用CLCoordinatorForMsgLoop对象的析构函数
    CLCoordinatorForMsgLoop threadForMsgLoop(new CLMyMsgProcessor(),"adder",true);
    threadForMsgLoop.Run(0);

    CLExecutiveNameServer::PostExecutiveMessage("adder",new CLAddMessage(2,4));
    CLExecutiveNameServer::PostExecutiveMessage("adder",new CLAddMessage(3,6));
    CLExecutiveNameServer::PostExecutiveMessage("adder",new CLAddMessage(5,6));
    CLExecutiveNameServer::PostExecutiveMessage("adder",new CLQuitMessage());
}
