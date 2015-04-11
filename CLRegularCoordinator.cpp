#include <iostream>
#include "CLRegularCoordinator.h"
#include "CLExecutive.h"
#include "CLExecutiveFunctionProvider.h"
#include <iostream>
using std::cout;
using std::endl;

CLRegularCoordinator::CLRegularCoordinator(){}

CLRegularCoordinator::~CLRegularCoordinator(){
    cout<<"CLRegularCoordinator::ReturnControlRights()"<<endl;
}

CLStatus CLRegularCoordinator::Run(void* pContext){
    if(m_pExecutive == NULL || m_pFunctionProvider == NULL)
	return CLStatus(-1,0);

    m_pContext = pContext;

    return m_pExecutive->Run();
}

CLStatus CLRegularCoordinator::ReturnControlRights(){
    return m_pFunctionProvider->RunExecutiveFunction(m_pContext);
}

CLStatus CLRegularCoordinator::WaitForDeath(){
    return m_pExecutive->WaitForDeath();
}
