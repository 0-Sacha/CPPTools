#pragma once


// Target Define
#ifdef BASE_TARGET_DEBUG
	#define CPPTOOLS_DEBUG
#endif

#ifdef BASE_TARGET_RELEASE
	#define CPPTOOLS_RELEASE
#endif

#ifdef BASE_TARGET_DIST
	#define CPPTOOLS_DIST
#endif


// Architecture Define
#ifdef BASE_ARCHITECTURE_X86
	#define CPPTOOLS_X86
#endif

#ifdef BASE_ARCHITECTURE_X86_64
	#define CPPTOOLS_X64
#endif


// Platform Define 
#ifdef BASE_PLATFORM_WINDOWS
	#define CPPTOOLS_PLATFORM_WINDOWS
#endif

#ifdef BASE_PLATFORM_LINUX
	#error CPPTools doesn't support Linux yet
#endif


// Compiler Define
#ifdef BASE_COMPILER_MSVC
#endif

#ifdef BASE_COMPILER_GCC
	#error CPPTools doesn't support GCC yet
#endif


// Include 
#include <utility>
#include <limits>
#include <array>
#include <bitset>
#include <thread>
#include <string>
#include <chrono>
#include <algorithm>
#include <functional>

#include <fstream>