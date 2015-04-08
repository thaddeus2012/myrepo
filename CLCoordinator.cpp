#include "CLCoordinator.h"

CLCoordinator::CLCoordinator(){}

CLCoordinator::~CLCoordinator(){
    delete m_pFunctionProvider;
}

void CLCoordinator::SetExecObjects(CLExecutive* pExecutive, CLExecutiveFunctionProvider* pFunctionProvider){
    m_pExecutive = pExecutive;
    m_pFunctionProvider = pFunctionProvider;
}
