#include <iostream>

class A
{
public:
	A() {}
	~A() {
		throw 42;
	}
};

int main()
{
	try {
		A a;
		throw 32;
	}
	catch (int e)
	{
		std::cout << e << std::endl;
	}
	system("pause");
	return 0;
}
