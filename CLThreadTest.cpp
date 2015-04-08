#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLRegularCoordinator.h"
#include "CLCriticalSection.h"
#include "CLConditionVariable.h"
#include "CLEvent.h"

using std::cout;
using std::endl;

struct SPara{
    int flag;
    CLMutex mutex;
    CLConditionVariable condition;
};

class CLMyFunction: public CLExecutiveFunctionProvider{
    public:
	CLMyFunction(){}
	virtual ~CLMyFunction(){}

	//void test(){
	//    throw 32;
	//}

	virtual CLStatus RunExecutiveFunction(void* pContext){
	    try{
	        CLEvent* p = (CLEvent*)pContext;

		sleep(2);
		p->Set();
		//CLCriticalSection(&(p->mutex));
		//p->flag++;
	        //cout<<"In thread("<<pthread_self()<<") "<<"flag="<<p->flag<<endl;
		//test();

		return CLStatus(0,0);
	    }catch(...){
		cout<<"Exception"<<endl;
	    }
	    return CLStatus(0,0);
	}
};

int main(){
    CLEvent* p = new CLEvent();

    CLCoordinator* pCoordinator = new CLRegularCoordinator();
    CLExecutive* pExecutive = new CLThread(pCoordinator);
    CLExecutiveFunctionProvider* pProvider = new CLMyFunction();
    pCoordinator->SetExecObjects(pExecutive,pProvider);

    //CLCoordinator* pCoordinator2 = new CLRegularCoordinator();
    //CLExecutive* pExecutive2 = new CLThread(pCoordinator2);
    //CLExecutiveFunctionProvider* pProvider2 = new CLMyFunction();
    //pCoordinator2->SetExecObjects(pExecutive2,pProvider2);

    pCoordinator->Run((void*)p);
    //pCoordinator2->Run((void*)p);


    p->Wait();


    pCoordinator->WaitForDeath();
    //pCoordinator2->WaitForDeath();

    return 0;
}
