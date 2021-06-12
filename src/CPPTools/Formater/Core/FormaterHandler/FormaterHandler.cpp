
#include "CPPToolspch.h"
#include "FormaterHandler.h"

namespace CPPTools::Fmt {

	FormaterHandler& FormaterHandler::GetInstance()
	{
		return m_Instance;
	}

	FormaterHandler::FormaterHandler()
	{
		SetBeginTime();
	}

	void FormaterHandler::SetBeginTime()
	{
		GetInstance().m_TimeShift = std::chrono::high_resolution_clock::now();
	}

	void FormaterHandler::SetHoursShift(const std::chrono::hours& hours)
	{
		GetInstance().m_HoursShift = hours;
	}

	const std::chrono::time_point<std::chrono::high_resolution_clock>& FormaterHandler::GetTimeShift()
	{
		return GetInstance().m_TimeShift;
	}

	const std::chrono::hours& FormaterHandler::GetHoursShift()
	{
		return GetInstance().m_HoursShift;
	}

}

CPPTools::Fmt::FormaterHandler CPPTools::Fmt::FormaterHandler::m_Instance;