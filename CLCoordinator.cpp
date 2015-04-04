#include "CLCoordinator.h"

CLCoordinator::CLCoordinator(){}

CLCoordinator::~CLCoordinator(){}

void CLCoordinator::SetExecObjects(CLExecutive* pExecutive, CLExecutiveFunctionProvider* pFunctionProvider){
    m_pExecutive = pExecutive;
    m_pFunctionProvider = pFunctionProvider;
}
