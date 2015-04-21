#ifndef CLCOORDINATORFORMSGLOOP_H
#define CLCOORDINATORFORMSGLOOP_H

#include "CLCoordinator.h"

#include "CLMessageObserver.h"

class CLCoordinatorForMsgLoop: public CLCoordinator{
    public:
	CLCoordinatorForMsgLoop(CLMessageObserver*, const char*);
	CLCoordinatorForMsgLoop(CLMessageObserver*, const char*, bool);
	virtual ~CLCoordinatorForMsgLoop();

	virtual CLStatus Run(void* pContext);
	virtual CLStatus ReturnControlRights();
	virtual CLStatus WaitForDeath();

    private:
	CLCoordinatorForMsgLoop(const CLCoordinatorForMsgLoop&);
	CLCoordinatorForMsgLoop& operator=(const CLCoordinatorForMsgLoop&);

    private:
	void* m_pContext;
	bool m_bWaitForDeath;

};
#endif
