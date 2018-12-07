#ifndef DLLOADER_HPP
#define DLLOADER_HPP

#include "IDLLoader.hpp"
#include <dlfcn.h>
#include <iostream>

namespace dlloader {

	template <class T>
	class DLLoader : public IDLLoader<T> {

	private:
		void *_handle;
		std::string _pathToLib;
		std::string _allocClassSymbol;

	public:
		DLLoader(std::string const &pathToLib, std::string const &allocClassSymbol)
		    : _handle(nullptr), _pathToLib(pathToLib), _allocClassSymbol(allocClassSymbol)
		{
		}

		~DLLoader() = default;

		void DLOpenLib()
		{
			if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
				std::cerr << dlerror() << std::endl;
			}
		}

		std::unique_ptr<T> DLGetInstance() override
		{
			using allocClass = T *(*)();

			auto allocFunc = reinterpret_cast<allocClass>(dlsym(_handle, _allocClassSymbol.c_str()));

			if (!allocFunc) {
				DLCloseLib();
				std::cerr << dlerror() << std::endl;
			}

			return std::unique_ptr<T>(allocFunc());
		}

		void DLCloseLib() override
		{
			if (dlclose(_handle) != 0) {
				std::cerr << dlerror() << std::endl;
			}
		}
	};

} // namespace dlloader

#endif
