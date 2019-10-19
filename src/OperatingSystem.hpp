#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#include <Windows.h>

#elif defined(USE_LINUX_OPERATING_SYSTEM)
#include <dlfcn.h>
#include <cstdlib>

#endif

namespace OS {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	using LibraryHandle = HMODULE;
#elif defined(USE_LINUX_OPERATING_SYSTEM)
	using LibraryHandle = void*;
#endif
}

#endif