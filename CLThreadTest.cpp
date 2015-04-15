#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLRegularCoordinator.h"
#include "CLCriticalSection.h"
#include "CLConditionVariable.h"
#include "CLEvent.h"
#include "CLMessageQueueBySTLQueue.h"
#include "CLMsgLoopManagerForSTLQueue.h"
#include "CLMessageObserver.h"

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

class CLAdder: public CLExecutiveFunctionProvider{
    public:
	CLAdder(CLMessageLoopManager* pMsgLoopManager){
	    if(pMsgLoopManager == NULL)
		throw "In CLAdder::CLAdder(), pMsgLoopManager error";

	    m_pMsgLoopManager = pMsgLoopManager;
	}
	virtual ~CLAdder(){
	    cout<<"CLAdder::~CLAdder()"<<endl;
	    if(m_pMsgLoopManager != NULL)
		delete m_pMsgLoopManager;
	}

	virtual CLStatus RunExecutiveFunction(void* pContext){
	    return m_pMsgLoopManager->EnterMessageLoop(pContext);
	}

    private:
	CLMessageLoopManager* m_pMsgLoopManager;
};

int main(){
    CLMessageQueueBySTLQueue* pQ = new CLMessageQueueBySTLQueue();
    CLMyMsgProcessor* pMsgProcessor = new CLMyMsgProcessor();
    CLMessageLoopManager* pM = new CLMsgLoopManagerForSTLQueue(pMsgProcessor,pQ);

    CLCoordinator* pCoordinator = new CLRegularCoordinator();
    CLExecutive* pExecutive = new CLThread(pCoordinator,true);
    //CLExecutive* pExecutive = new CLThread(pCoordinator);
    CLExecutiveFunctionProvider* pProvider = new CLAdder(pM);
    pCoordinator->SetExecObjects(pExecutive,pProvider);

    pCoordinator->Run(0);

    CLAddMessage* paddmsg = new CLAddMessage(2,4);
    pQ->PushMessage(paddmsg);

    CLAddMessage* paddmsg1 = new CLAddMessage(3,6);
    pQ->PushMessage(paddmsg1);

    CLAddMessage* paddmsg2 = new CLAddMessage(5,6);
    pQ->PushMessage(paddmsg2);

    CLQuitMessage* pquitmsg = new CLQuitMessage();
    pQ->PushMessage(pquitmsg);

    pCoordinator->WaitForDeath();
    //sleep(3);

    return 0;
}
