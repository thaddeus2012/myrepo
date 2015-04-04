#include <iostream>
#include "CLThread.h"

using std::cout;
using std::endl;

class CLParaPrinter: public CLExecutiveFunctionProvider{
    public:
	CLParaPrinter(){}
	~CLParaPrinter(){}

	virtual CLStatus RunExecutiveFunction(void* pContext){
	    long i = (long)pContext;
	    cout<<i<<endl;
	    return CLStatus(0,0);
	}
};

int main(){
    CLExecutiveFunctionProvider* printer = new CLParaPrinter();
    CLExecutive* pExecutive = new CLThread(printer);

    pExecutive->Run((void*)5);
    pExecutive->WaitForDeath();

    return 0;
}
