#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLRegularCoordinator.h"

using std::cout;
using std::endl;

class CLMyPrinter: public CLExecutiveFunctionProvider{
    public:
	CLMyPrinter(){}
	~CLMyPrinter(){}

	virtual CLStatus RunExecutiveFunction(void* pContext){
	    long i = (long)pContext;
	    cout<<"In thread("<<pthread_self()<<") "<<i<<endl;
	    return CLStatus(0,0);
	}
};

int main(){
    CLCoordinator* pCoordinator = new CLRegularCoordinator();
    CLExecutive* pExecutive = new CLThread(pCoordinator);
    CLExecutiveFunctionProvider* pProvider = new CLMyPrinter();

    pCoordinator->SetExecObjects(pExecutive,pProvider);

    pCoordinator->Run((void*)5);
    cout<<"In main thread("<<pthread_self()<<") "<<endl;
    pCoordinator->WaitForDeath();

    return 0;
}
