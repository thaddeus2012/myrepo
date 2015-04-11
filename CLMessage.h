#ifndef CLMESSAGE_H
#define CLMESSAGE_H

class CLMessage{
    public:
	CLMessage(unsigned long);
	virtual ~CLMessage();

    public:
	const unsigned long& m_clMsgID;
    
    private:
	CLMessage(const CLMessage&);
	CLMessage& operator=(const CLMessage&);

    protected:
	unsigned long m_lMsgID;
};
#endif
