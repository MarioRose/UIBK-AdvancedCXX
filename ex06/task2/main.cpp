#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <dlfcn.h>
#include "plugin.hpp"


void runPlugin(const std::string& path)
{
	// load the triangle library
  void* plugin = dlopen(path.c_str(), RTLD_LAZY);
  if (!plugin) {
      std::cerr << "Cannot load library: " << path.c_str() << dlerror() << '\n';
      return;
  }

  // reset errors
  dlerror();

  // load the symbols
  create_t* create_plugin = (create_t*) dlsym(plugin, PLUGIN_CONSTRUCTOR);
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
      std::cerr << "Cannot load symbol create: " << PLUGIN_CONSTRUCTOR << dlsym_error << '\n';
      return;
  }

  // create an instance of the class
  std::unique_ptr<Plugin> p = create_plugin();
	p->run();
  p.reset();
	dlclose(plugin);

}


int main(int argc, char *argv[])
{

	if(argc < 2) {
		std::cout << "Please provide plugins as arguments" << std::endl;
		return -1;
	}

	std::vector<std::string> arguments(argv + 1, argv + argc);

	for(std::string& s : arguments) {
	    runPlugin(s);
	}

	return 0;
}
