#include "CLExecutive.h"

CLExecutive::CLExecutive(CLCoordinator* pCoordinator){
    if(pCoordinator == 0)
	throw "In CLExecutive::CLExecutive(), pCoordinator error";

    m_pCoordinator = pCoordinator;
}

CLExecutive::~CLExecutive(){}
