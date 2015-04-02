#ifndef CLEXCUTIVE_H
#define CLEXCUTIVE_H

#include "CLExcutiveFunctionProvider.h"

class CLExcutive{
    public:
	explicit CLExcutive(CLExcutiveFunctionProvider*);
	virtual ~CLExcutive();

	virtual CLStatus Run(void* pContext) = 0;
	virtual CLStatus WaitForDeath() = 0;

    protected:
	CLExcutiveFunctionProvider* m_pExcutiveFunctionProvider;

    private:
	CLExcutive(const CLExcutive&);
	CLExcutive& operator=(const CLExcutive&);
};
#endif
