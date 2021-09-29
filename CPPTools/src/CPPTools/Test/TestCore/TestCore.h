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

	struct TestMemoryCollisionAvoid
	{
		TestMemoryCollisionAvoid()
			: Link(nullptr)
			, NbTest(0)
			, NbValidTest(0)
			, NbFailTest(0)
		{}

		TestMemoryCollisionAvoid(TestMemoryCollisionAvoid& link)
			: Link(&link)
			, NbTest(0)
			, NbValidTest(0)
			, NbFailTest(0)
		{}

		~TestMemoryCollisionAvoid() {
			if (Link != nullptr)
			{
				Link->NbTest += NbTest;
				Link->NbValidTest += NbValidTest;
				Link->NbFailTest += NbFailTest;
			}
		}

		void AddValid() {
			NbTest++;
			NbValidTest++;
		}

		void AddFail() {
			NbTest++;
			NbFailTest++;
		}

	public:
		TestMemoryCollisionAvoid* Link;
		std::uint16_t NbTest;
		std::uint16_t NbValidTest;
		std::uint16_t NbFailTest;
	};

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

template<typename FormatContext>
struct CPPTools::Fmt::FormatType<CPPTools::Test::TestMemoryCollisionAvoid, FormatContext>
{
	static void Write(const CPPTools::Test::TestMemoryCollisionAvoid& val, FormatContext& context) {
		if (val.NbFailTest == 0)	context.LittleFormat("{C:green}{} tests fails and {} valid over {} tests", val.NbFailTest, val.NbValidTest, val.NbTest);
		else 						context.LittleFormat("{C:red}{} tests valid over {} tests ({} tests fail)", val.NbValidTest, val.NbTest, val.NbFailTest);
	}
};
