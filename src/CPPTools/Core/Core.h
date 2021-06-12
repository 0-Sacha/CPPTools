#pragma once

#include "CPPToolspch.h"

#ifdef CPPTOOLS_DEBUG
	
	#define PROFILING_ENABLE

	#define MEMORY_PROFILING_ENABLE
	#define MEMORY_PROFILER_NEW

#endif

namespace CPPTools {
	namespace Experimental {

	}

	namespace Deprecated {

	}
}



#define CPPTOOLS_NODISCARD [[nodiscard]]