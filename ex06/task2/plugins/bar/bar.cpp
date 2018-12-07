#include "bar.hpp"

extern "C"
{
	std::unique_ptr<Bar> create_plugin()
	{
		return std::unique_ptr<Bar>(new Bar());
	}
}

void Bar::run()
{
	std::cout << "Running bar" << std::endl;
}
