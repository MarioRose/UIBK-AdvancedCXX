#include "intercept_foo.hpp"

#include <iostream>
#include <dlfcn.h>


namespace foo {

int random_number()
{
	return 4;
}

void print_before()
{
	std::cout << "some text before\n";
}

void just_a_print()
{
	std::cout << "some text before\n";

	void *handle;

	handle = dlopen("./libfoo.so", RTLD_NOW | RTLD_GLOBAL);
	    if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	    }

	void (*real_print)() = (void(*)())dlsym(handle, "just_a_print");

	if (!real_print){
		std::cerr << dlerror() << std::endl;
		return;
	}

	if(real_print == NULL){
		std::cout << "some text before\n";
	}
	real_print();

	std::cout << "some text after\n";
}

} // end namespace foo
