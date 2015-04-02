#ifndef CLSTATUS_H
#define CLSTATUS_H
class CLStatus{
    public:
	CLStatus(long lReturnCode,long lErrorCode);
	CLStatus(const CLStatus& s);
	~CLStatus();

	bool IsSuccess();

	const long& m_clReturnCode;
	const long& m_clErrorCode;

    private:
	long m_lReturnCode;
	long m_lErrorCode;
};
#endif
