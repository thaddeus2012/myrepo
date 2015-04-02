#ifndef CLEXCUTIVEFUNCTIONPROVIDER_H
#define CLEXCUTIVEFUNCTIONPROVIDER_H

#include "CLStatus.h"

class CLExcutiveFunctionProvider{
    public:
	CLExcutiveFunctionProvider();
	~CLExcutiveFunctionProvider();

    public:
	virtual CLStatus RunExcutiveFunction(void* pContext) = 0;

    private:
	CLExcutiveFunctionProvider(const CLExcutiveFunctionProvider&);
	CLExcutiveFunctionProvider& operator=(const CLExcutiveFunctionProvider&);
};

#endif
