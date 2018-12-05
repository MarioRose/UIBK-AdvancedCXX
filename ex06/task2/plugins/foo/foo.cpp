#include "foo.hpp"

extern "C"
{
	Foo *create_plugin()
	{
		return new Foo();
	}

	void deleter(Foo *ptr)
	{
		delete ptr;
	}
}

void Foo::run()
{
	std::cout << "Running foo" << std::endl;
}
