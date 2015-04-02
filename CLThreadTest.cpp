#include <iostream>
#include "CLThread.h"

using std::cout;
using std::endl;

class CLParaPrinter: public CLExcutiveFunctionProvider{
    public:
	CLParaPrinter(){}
	~CLParaPrinter(){}

	virtual CLStatus RunExcutiveFunction(void* pContext){
	    long i = (long)pContext;
	    cout<<i<<endl;
	    return CLStatus(0,0);
	}
};

int main(){
    CLExcutiveFunctionProvider* printer = new CLParaPrinter();
    CLExcutive* pExcutive = new CLThread(printer);

    pExcutive->Run((void*)5);
    pExcutive->WaitForDeath();

    return 0;
}
