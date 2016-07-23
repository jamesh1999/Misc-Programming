#ifndef __Register_INCLUDED__
#define __Register_INCLUDED__

//Represents a single register in the CPU

class alignas(4) Register
{
private:
	unsigned m_value;
public:
	Register(){ clear(); }; //Initialize to 0

	void clear()
	{
		m_value = 0;
	};

	unsigned getValue() const
	{
		return m_value;
	};

	void setValue(const unsigned& val)
	{
		m_value = val;
	};
};

#endif