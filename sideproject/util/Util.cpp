#include "Util.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

namespace util {
    std::vector<std::string> getValues(std::string value)
    {

        std::stringstream ss(value);
    	std::vector<std::string> result;

    	while (ss.good()) {
    		std::string substr;
    		getline(ss, substr, ',');
    		result.push_back(substr);
    	}

        return result;

    }
}
