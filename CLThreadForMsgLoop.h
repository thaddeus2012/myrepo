#ifndef CLTHREADFORMSGLOOP_H
#define CLTHREADFORMSGLOOP_H

class CLThreadForMsgLoop{
    public:
	CLThreadForMsgLoop(CLThread*);

    private:
	CLthread* m_pThread;
	bool m_bWaitForDeath;
};
#endif
