#pragma once


// Target Define
#ifdef BASE_TARGET_DEBUG
	#define CPPTOOLS_DEBUG
	#define CPPTOOLS_IF_DEBUG(x) x
#else
	#define CPPTOOLS_IF_DEBUG(x)
#endif

#ifdef BASE_TARGET_RELEASE
	#define CPPTOOLS_RELEASE
	#define CPPTOOLS_IF_RELEASE(x) x
#else
	#define CPPTOOLS_IF_RELEASE(x)
#endif

// Architecture Define
#ifdef BASE_ARCHITECTURE_X86
	#define CPPTOOLS_X86
	#define CPPTOOLS_IF_X86(x) x
#else
	#define CPPTOOLS_IF_X86(x)
#endif

#ifdef BASE_ARCHITECTURE_X64
	#define CPPTOOLS_X64
	#define CPPTOOLS_IF_X64(x) x
#else
	#define CPPTOOLS_IF_X64(x)
#endif


// Platform Define 
#ifdef BASE_PLATFORM_WINDOWS
	#define CPPTOOLS_WINDOWS
	#define CPPTOOLS_IF_WINDOWS(x) x
#else
	#define CPPTOOLS_IF_WINDOWS(x)
#endif

#ifdef BASE_PLATFORM_LINUX
	#define CPPTOOLS_IF_LINUX(x) x
#else
	#define CPPTOOLS_IF_LINUX(x)
#endif


// Compiler Define
#ifdef BASE_COMPILER_MSVC
	#define CPPTOOLS_IF_MSVC(x) x
#else
	#define CPPTOOLS_IF_MSVC(x)
#endif

#ifdef BASE_COMPILER_GCC
	#define CPPTOOLS_IF_GCC(x) x
#else
	#define CPPTOOLS_IF_GCC(x)
#endif


// Include 
#include <cstring>

#include <utility>
#include <limits>
#include <array>
#include <bitset>
#include <thread>
#include <string>
#include <chrono>
#include <algorithm>
#include <functional>
#include <type_traits>

#include <fstream>

