#include "foo.hpp"

extern "C" Foo *create_plugin()
{
	return new Foo();
}

void Foo::run()
{
	std::cout << "Running foo" << std::endl;
}
