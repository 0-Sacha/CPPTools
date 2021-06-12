#pragma once

#include "CPPTools/LogSystem/LogSystem.h"

#define UNIT_TEST_TRACE(...)	CPPTools::UnitTest::GetLogger().LogTrace(__VA_ARGS__)
#define UNIT_TEST_INFO(...)		CPPTools::UnitTest::GetLogger().LogInfo(__VA_ARGS__)
#define UNIT_TEST_WARN(...)		CPPTools::UnitTest::GetLogger().LogWarn(__VA_ARGS__)
#define UNIT_TEST_ERROR(...)	CPPTools::UnitTest::GetLogger().LogError(__VA_ARGS__)
#define UNIT_TEST_FATAL(...)	CPPTools::UnitTest::GetLogger().LogFatal(__VA_ARGS__)

#define UNIT_TEST_OK(...)		CPPTools::UnitTest::GetLogger().LogOk(__VA_ARGS__)
#define UNIT_TEST_FAIL(...)		CPPTools::UnitTest::GetLogger().LogFail(__VA_ARGS__)


namespace CPPTools {
	class UnitTest
	{
	public:
		static LogSystem& GetLogger();
	};
}
