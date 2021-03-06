#ifndef BAR_HPP
#define BAR_HPP

#include <iostream>
#include "plugin.hpp"

class Bar : public Plugin
{
public:

	Bar() {
		std::cout << "Creation of Bar" << std::endl;
	}

	~Bar() {
		std::cout << "Destruction of Bar" << std::endl;
	}

	void run() override;
};

#endif
