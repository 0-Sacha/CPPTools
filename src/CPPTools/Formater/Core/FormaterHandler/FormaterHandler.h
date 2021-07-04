#pragma once

#include <chrono>

namespace CPPTools::Fmt {

	class FormaterHandler {

	public:
		static FormaterHandler& GetInstance();

	public:
		static void SetBeginTime();
		static void SetHoursShift(const std::chrono::hours& hoursShift);
		static const std::chrono::time_point<std::chrono::high_resolution_clock>& GetTimeShift();
		static const std::chrono::hours& GetHoursShift();

	private:
		FormaterHandler();
		~FormaterHandler() = default;

		FormaterHandler(const FormaterHandler& other) = delete;

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimeShift;
		std::chrono::hours m_HoursShift;
		static FormaterHandler m_Instance;
	};
}

#define CPPTOOLS_FORMATER_TIME_BEGIN()	CPPTools::Fmt::FormaterHandler::SetBeginTime()

#define CPPTOOLS_FORMATER_SET_TIME_GTM(i) CPPTools::Fmt::FormaterHandler::SetHoursShift(std::chrono::hours(i))