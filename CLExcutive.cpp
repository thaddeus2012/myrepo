#include "CLExcutive.h"

CLExcutive::CLExcutive(CLExcutiveFunctionProvider* pExcutiveFunctionProvider){
    if(pExcutiveFunctionProvider == 0)
	throw "In CLExecutive::CLExecutive(), pExecutiveFunctionProv id error";

    m_pExcutiveFunctionProvider = pExcutiveFunctionProvider;
}

CLExcutive::~CLExcutive(){}
