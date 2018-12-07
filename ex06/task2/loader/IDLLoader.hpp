#ifndef IDLLOADER_HPP
#define IDLLOADER_HPP

#include <memory>
#include <string>

namespace dlloader
{
	/*
	** Interface for Dynamic Library Loading (DLLoader)
	*/
	template <class T>
	class IDLLoader
	{

	public:

		virtual ~IDLLoader() = default;

		virtual void DLOpenLib() = 0;

		/*
		** Return a shared pointer on an instance of class loaded through
		** a dynamic library.
		*/
		virtual std::unique_ptr<T>	DLGetInstance() = 0;

		/*
		** Correctly delete the instance of the "dynamically loaded" class.
		*/
		virtual void DLCloseLib() = 0;

	};
}

#endif
