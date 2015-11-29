#ifndef __REGISTER_INCLUDED__
#define __REGISTER_INCLUDED__


class Register
{
private:
	unsigned m_value;
public:
	Register();

	void clear();
	unsigned getValue();
	void setValue(unsigned);
	void increment();
};

#endif