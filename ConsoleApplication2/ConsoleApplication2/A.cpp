#include "stdafx.h"
#include "A.h"
#include "B.h"
namespace X {
	namespace Bar {
		A::A()
		{
			X::Foo::B* b = new X::Foo::B;
		}


		A::~A()
		{
		}
	}
}