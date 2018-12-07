#include "foo.hpp"

extern "C"
{
	std::unique_ptr<Foo> create_plugin()
	{
		return std::unique_ptr<Foo>(new Foo());
	}
}

void Foo::run()
{
	std::cout << "Running foo" << std::endl;
}
