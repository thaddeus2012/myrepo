#include "CLExecutive.h"
#include <iostream>
using std::cout;
using std::endl;

CLExecutive::CLExecutive(CLCoordinator* pCoordinator){
    if(pCoordinator == 0)
	throw "In CLExecutive::CLExecutive(), pCoordinator error";

    m_pCoordinator = pCoordinator;
}

CLExecutive::~CLExecutive(){
    cout<<"CLExecutive::~CLExecutive()"<<endl;
    //delete m_pCoordinator; //Coordinator对象声明为栈上对象自动销毁
}
