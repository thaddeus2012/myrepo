#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLRegularCoordinator.h"
#include "CLCriticalSection.h"

using std::cout;
using std::endl;

struct SPara{
    int flag;
    CLMutex mutex;
};

class CLMyFunction: public CLExecutiveFunctionProvider{
    public:
	CLMyFunction(){}
	virtual ~CLMyFunction(){}

	void test(){
	    throw 32;
	}

	virtual CLStatus RunExecutiveFunction(void* pContext){
	    try{
	        SPara* p = (SPara*)pContext;

		CLCriticalSection(&(p->mutex));
		p->flag++;
	        cout<<"In thread("<<pthread_self()<<") "<<"flag="<<p->flag<<endl;
		test();
	    }catch(...){
		cout<<"Exception"<<endl;
	    }
	    return CLStatus(0,0);
	}
};

int main(){
    CLCoordinator* pCoordinator = new CLRegularCoordinator();
    CLExecutive* pExecutive = new CLThread(pCoordinator);
    CLExecutiveFunctionProvider* pProvider = new CLMyFunction();

    pCoordinator->SetExecObjects(pExecutive,pProvider);

    CLCoordinator* pCoordinator2 = new CLRegularCoordinator();
    CLExecutive* pExecutive2 = new CLThread(pCoordinator2);
    CLExecutiveFunctionProvider* pProvider2 = new CLMyFunction();

    pCoordinator2->SetExecObjects(pExecutive2,pProvider2);

    SPara *p = new SPara;
    p->flag = 3;

    pCoordinator->Run((void*)p);
    pCoordinator2->Run((void*)p);

    sleep(2);

    p->mutex.Lock();

    p->flag++;
    cout<<"In main thread("<<pthread_self()<<") "<<"flag="<<p->flag<<endl;

    p->mutex.UnLock();

    pCoordinator->WaitForDeath();
    pCoordinator2->WaitForDeath();

    return 0;
}
