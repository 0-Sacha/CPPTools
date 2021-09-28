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

#define CPPTOOLS_TEST_SIMPE_MSG()	CPPTools::Test::TestCore::GetLogger().SetBaseFormat("{name} >> {data}");


namespace CPPTools::Test {

	struct TestCollisionAvoider {};

	class TestCore
	{
	public:
		static LogSystem& GetLogger() {
			static LogSystem instance("TEST");
			return instance;
		}
	};

	constexpr std::string_view GetFileName(const char* path) {
		const char* begin = nullptr;
		const char* end = nullptr;

		while (*path) {
			if (*path == '/' || *path == '\\')
				begin = ++path;
			else if (*path == '.')
				end = path++;
			else
				++path;
		}
		return std::string_view(begin, end - begin);
	}
}
