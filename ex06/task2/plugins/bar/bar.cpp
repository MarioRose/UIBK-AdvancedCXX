#include "bar.hpp"

extern "C" Bar *create_plugin()
{
	return new Bar();
}

void Bar::run()
{
	std::cout << "Running bar" << std::endl;
}
