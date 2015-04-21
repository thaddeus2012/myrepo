#ifndef CLEXECUTIVECOMMUNICATION_H
#define CLEXECUTIVECOMMUNICATION_H

#include "CLStatus.h"

class CLMessage;

class CLExecutiveCommunication{
    public:
	CLExecutiveCommunication();
	virtual ~CLExecutiveCommunication();

	virtual CLStatus PostExecutiveMessage(CLMessage*) = 0;

    private:
	CLExecutiveCommunication(const CLExecutiveCommunication&);
	CLExecutiveCommunication& operator=(const CLExecutiveCommunication&);
};
#endif
