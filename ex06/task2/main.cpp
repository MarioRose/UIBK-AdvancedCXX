#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "DLLoader.hpp"
#include "plugin.hpp"

/*
** Using the smart pointer directly in an inner function because
** the reference to its destructor contained in the dll is lost before
** going out of the caller function's scope.
*/
void runPlugin(dlloader::DLLoader<Plugin>& dlloader)
{
	std::shared_ptr<Plugin> plugin = dlloader.DLGetInstance();

	plugin->run();
}

void run(const std::string& path)
{
	dlloader::DLLoader<Plugin> dlloader(path, PLUGIN_CONSTRUCTOR);

	dlloader.DLOpenLib();
	runPlugin(dlloader);
	dlloader.DLCloseLib();
}

int main(int argc, char *argv[])
{

	if(argc < 2) {
		std::cout << "Please provide plugins as arguments" << std::endl;
		return -1;
	}

	std::vector<std::string> arguments(argv + 1, argv + argc);

	for(std::string& s : arguments) {
	    run(s);
	}

	return 0;
}
