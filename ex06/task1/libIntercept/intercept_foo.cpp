#include "intercept_foo.hpp"

#include <iostream>
#include <dlfcn.h>
#include <cstring>


namespace foo {

int random_number()
{
	return 4;
}

void just_a_print()
{
	typedef void (*handle)();

	std::cout << "some text before\n";

	handle print = (handle)dlsym(RTLD_NEXT, "_ZN3foo12just_a_printEv");
	(*print)();

	std::cout << "some text after\n";
}

} // end namespace foo
