#pragma once

#include "CPPTools/LogSystem/LogSystem.h"

#define UNITTEST_TRACE(...)	CPPTools::UnitTest::GetLogger().LogTrace(__VA_ARGS__)
#define UNITTEST_INFO(...)	CPPTools::UnitTest::GetLogger().LogInfo(__VA_ARGS__)
#define UNITTEST_WARN(...)	CPPTools::UnitTest::GetLogger().LogWarn(__VA_ARGS__)
#define UNITTEST_ERROR(...)	CPPTools::UnitTest::GetLogger().LogError(__VA_ARGS__)
#define UNITTEST_FATAL(...)	CPPTools::UnitTest::GetLogger().LogFatal(__VA_ARGS__)

#define UNITTEST_OK(...)	CPPTools::UnitTest::GetLogger().LogOk(__VA_ARGS__)
#define UNITTEST_FAIL(...)	CPPTools::UnitTest::GetLogger().LogFail(__VA_ARGS__)


namespace CPPTools {
	class UnitTest
	{
	public:
		static LogSystem& GetLogger();
	};
}
