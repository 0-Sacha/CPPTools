#pragma once

#include "CPPTools/LogSystem/LogSystem.h"

#define CPPTOOLS_TEST_TRACE(...)	CPPTools::Test::TestCore::GetLogger().LogTrace(__VA_ARGS__)
#define CPPTOOLS_TEST_INFO(...)		CPPTools::Test::TestCore::GetLogger().LogInfo(__VA_ARGS__)
#define CPPTOOLS_TEST_WARN(...)		CPPTools::Test::TestCore::GetLogger().LogWarn(__VA_ARGS__)
#define CPPTOOLS_TEST_ERROR(...)	CPPTools::Test::TestCore::GetLogger().LogError(__VA_ARGS__)
#define CPPTOOLS_TEST_FATAL(...)	CPPTools::Test::TestCore::GetLogger().LogFatal(__VA_ARGS__)

#define CPPTOOLS_TEST_OK(...)		CPPTools::Test::TestCore::GetLogger().LogOk(__VA_ARGS__)
#define CPPTOOLS_TEST_FAIL(...)		CPPTools::Test::TestCore::GetLogger().LogFail(__VA_ARGS__)

#define CPPTOOLS_TEST_BASIC(...)	CPPTools::Test::TestCore::GetLogger().LogBasic(__VA_ARGS__)


namespace CPPTools::Test {
	class TestCore
	{
	public:
		static LogSystem& GetLogger()	{ static LogSystem instance("CPPTOOLS-Test"); return instance; }
	};
}
