#ifndef CLTHREADINITIALFINISHEDNOTIFIER_H
#define CLTHREADINITIALFINISHEDNOTIFIER_H

#include "CLExecutiveInitialFinishedNotifier.h"
#include "CLEvent.h"

class CLThreadInitialFinishedNotifier: public CLExecutiveInitialFinishedNotifier{
    public:
	CLThreadInitialFinishedNotifier(CLEvent*);
	virtual ~CLThreadInitialFinishedNotifier();

	virtual CLStatus NotifyInitialFinished(bool);
	virtual bool IsInitialSuccess();

    private:
	CLThreadInitialFinishedNotifier(const CLThreadInitialFinishedNotifier&);
	CLThreadInitialFinishedNotifier& operator=(const CLThreadInitialFinishedNotifier&);

    private:
	bool m_bSuccess;
	CLEvent* m_pEvent;
};
#endif
