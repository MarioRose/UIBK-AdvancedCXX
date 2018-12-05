#include "bar.hpp"

extern "C"
{
	Bar *create_plugin()
	{
		return new Bar();
	}

	void deleter(Bar *ptr)
	{
		delete ptr;
	}
}

void Bar::run()
{
	std::cout << "Running bar" << std::endl;
}
