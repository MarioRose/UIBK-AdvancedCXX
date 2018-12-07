#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>

/// Interface for plugins.
class Plugin {
  public:
	virtual void run() = 0;
	virtual ~Plugin(){};
};

/// Symbol of the plugin construtor function.
const auto PLUGIN_CONSTRUCTOR = "create_plugin";

/// Type of the plugin constructor function.
using PLUGIN_CONSTRUCTOR_T = std::unique_ptr<Plugin> (*)();

// the types of the class factories
typedef std::unique_ptr<Plugin> create_t();

#endif // PLUGIN_HPP
