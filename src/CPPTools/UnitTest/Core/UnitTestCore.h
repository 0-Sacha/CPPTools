#pragma once

#include "CPPTools/LogSystem/LogSystem.h"

#define UNITTEST_TRACE(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogTrace(__VA_ARGS__)
#define UNITTEST_INFO(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogInfo(__VA_ARGS__)
#define UNITTEST_WARN(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogWarn(__VA_ARGS__)
#define UNITTEST_ERROR(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogError(__VA_ARGS__)
#define UNITTEST_FATAL(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogFatal(__VA_ARGS__)

#define UNITTEST_OK(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogOk(__VA_ARGS__)
#define UNITTEST_FAIL(...)	CPPTools::UnitTest::UnitTestCore::GetLogger().LogFail(__VA_ARGS__)


namespace CPPTools::UnitTest {
	class UnitTestCore
	{
	public:
		static LogSystem& GetLogger()	{ static LogSystem instance("UnitTest"); return instance; }
	};
}
