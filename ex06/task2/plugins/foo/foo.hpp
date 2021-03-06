#ifndef FOO_HPP
#define FOO_HPP

#include <iostream>
#include "plugin.hpp"

class Foo : public Plugin
{
public:
	Foo() {
		std::cout << "Creation of Foo" << std::endl;
	}

	~Foo() {
		std::cout << "Destruction of Foo" << std::endl;
	}

	void run() override;
};

#endif
