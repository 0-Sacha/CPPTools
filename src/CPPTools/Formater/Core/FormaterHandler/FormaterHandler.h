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

#define GTM(i) (std::chrono::hours)i

#define RESET_STRFORMAT_API_TIME(i)	CPPTools::Fmt::Formater::GetAPI().SetBeginTime(); CPPTools::Fmt::Formater::GetAPI().SetHoursShift(i)