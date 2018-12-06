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
	std::cout << "some text before\n";

	typedef void (*handle)();

	static handle origMethod = 0;
	if(origMethod == 0) {
		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN3foo12just_a_printEv");

		std::memcpy(&origMethod, &tmpPtr, sizeof(void*));
	}

	(*origMethod)();

	std::cout << "some text after\n";
}

} // end namespace foo
