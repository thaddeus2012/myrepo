#include "CLCoordinator.h"
#include <iostream>
using std::cout;
using std::endl;

CLCoordinator::CLCoordinator(){}

CLCoordinator::~CLCoordinator(){
    cout<<"CLCoordinator::~CLCoordinator()"<<endl;
    delete m_pFunctionProvider;
}

void CLCoordinator::SetExecObjects(CLExecutive* pExecutive, CLExecutiveFunctionProvider* pFunctionProvider){
    m_pExecutive = pExecutive;
    m_pFunctionProvider = pFunctionProvider;
}
