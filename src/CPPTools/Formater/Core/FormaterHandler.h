#pragma once

#include <chrono>

namespace CPPTools::Fmt {

	class FormaterHandler {

	public:
		static FormaterHandler& GetInstance() { static FormaterHandler instance; return instance; }

	public:
		static void SetBeginTime()																	{ GetInstance().m_TimeShift = std::chrono::high_resolution_clock::now(); }
		static void SetHoursShift(const std::chrono::hours& hoursShift)								{ GetInstance().m_HoursShift = hoursShift; }
		static const std::chrono::time_point<std::chrono::high_resolution_clock>& GetTimeShift()	{ return GetInstance().m_TimeShift; }
		static const std::chrono::hours& GetHoursShift()											{ return GetInstance().m_HoursShift; }

	private:
		FormaterHandler() = default;
		~FormaterHandler() = default;

		FormaterHandler(const FormaterHandler& other) = delete;

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimeShift;
		std::chrono::hours m_HoursShift{};
	};
}

#define CPPTOOLS_FORMATER_TIME_BEGIN()	CPPTools::Fmt::FormaterHandler::SetBeginTime()

#define CPPTOOLS_FORMATER_SET_TIME_GTM(i) CPPTools::Fmt::FormaterHandler::SetHoursShift(std::chrono::hours(i))