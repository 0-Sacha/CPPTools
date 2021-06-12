
#include "CPPToolspch.h"
#include "UnitTestCore.h"

namespace CPPTools {

	LogSystem& UnitTest::GetLogger() {
		static LogSystem instance("UnitTest");
		return instance;
	}

}
