#ifndef CLEXECUTIVE_H
#define CLEXECUTIVE_H

#include "CLExecutiveFunctionProvider.h"

class CLExecutive{
    public:
	explicit CLExecutive(CLExecutiveFunctionProvider*);
	virtual ~CLExecutive();

	virtual CLStatus Run(void* pContext) = 0;
	virtual CLStatus WaitForDeath() = 0;

    protected:
	CLExecutiveFunctionProvider* m_pExecutiveFunctionProvider;

    private:
	CLExecutive(const CLExecutive&);
	CLExecutive& operator=(const CLExecutive&);
};
#endif
