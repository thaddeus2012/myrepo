#include "CLExecutive.h"

CLExecutive::CLExecutive(CLExecutiveFunctionProvider* pExecutiveFunctionProvider){
    if(pExecutiveFunctionProvider == 0)
	throw "In CLExecutive::CLExecutive(), pExecutiveFunctionProv id error";

    m_pExecutiveFunctionProvider = pExecutiveFunctionProvider;
}

CLExecutive::~CLExecutive(){}
