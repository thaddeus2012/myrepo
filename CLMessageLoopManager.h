#ifndef CLMESSAGELOOPMANAGER_H
#define CLMESSAGELOOPMANAGER_H

#include <map>
#include "CLStatus.h"
#include "CLLogger.h"
#include "CLMessage.h"
#include "CLMessageObserver.h"
#include "CLExecutiveInitialFinishedNotifier.h"

using std::map;

#define QUIT_MSG_LOOP 1

//CLMessageObserver继承体系类中对消息处理的回调函数类型
typedef CLStatus (CLMessageObserver::*CallBackForMessageLoop)(CLMessage*);

struct SLExecutiveInitialParameter{
    void* pContext;
    CLExecutiveInitialFinishedNotifier* pNotifier;
};

class CLMessageLoopManager{
    public:
	CLMessageLoopManager(CLMessageObserver*);
	virtual ~CLMessageLoopManager();

	virtual CLStatus EnterMessageLoop(void*);
	virtual CLStatus Register(unsigned long, CallBackForMessageLoop);

    protected:
	virtual CLStatus Initialize() = 0;
	virtual CLStatus Uninitialize() = 0;

	virtual CLMessage* WaitForMessage() = 0;
	virtual CLStatus DispatchMessage(CLMessage*);

    private:
	CLMessageLoopManager(const CLMessageLoopManager&);
	CLMessageLoopManager& operator=(const CLMessageLoopManager&);

    private:
	CLMessageObserver* m_pMessageObserver; //指向消息处理类对象
	//用消息处理类对象中的消息处理成员函数代替之前对应每种消
	//息的消息处理类对象指针
	map<unsigned long,CallBackForMessageLoop> m_MsgMappingTable;
};
#endif
