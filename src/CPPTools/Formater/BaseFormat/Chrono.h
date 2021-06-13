#pragma once

#include "../Formater.h"
#include <chrono>

namespace CPPTools::Fmt {

	template<typename Clock, typename Duration>
	void WriteTestTime(const std::chrono::time_point<Clock, Duration>& value, Formater& formater) {
		if (formater.FormatNextIsSame("ns"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count());
		else if (formater.FormatNextIsSame("us"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count());
		else if (formater.FormatNextIsSame("ms"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count());
		else if (formater.FormatNextIsSame("s"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count());
		else if (formater.FormatNextIsSame("m"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count());
		else if (formater.FormatNextIsSame("h"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count());
	}

	template<typename Clock, typename Duration>
	void WriteTestTimeMod(const std::chrono::time_point<Clock, Duration>& value, Formater& formater) {
		if (formater.FormatNextIsSame("ns"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (formater.FormatNextIsSame("us"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (formater.FormatNextIsSame("ms"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (formater.FormatNextIsSame("s"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count() % 60, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (formater.FormatNextIsSame("m"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count() % 60, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (formater.FormatNextIsSame("h"))
			FormatFunc::FormaterParseUInt<uint32_t>(formater, (uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count() % 24, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
	}

	template<typename Clock, typename Duration>
	bool WriteTime(const std::chrono::time_point<Clock, Duration>& value, Formater& formater) {
		bool isSpecified = false;
		if (formater.FormatIsEqNext(':')) {
			isSpecified = true;
			formater.WriteUntil('%', '#');
			while (!formater.FormatIsEndOfParameter()) {
				if (formater.FormatIsEqNext('%'))		WriteTestTimeMod(value, formater);
				else if (formater.FormatIsEqNext('#'))	WriteTestTime(value, formater);
				else									formater.FormatGoToEndOfParameter();
				formater.WriteUntil('%', '#');
			}
		}
		return isSpecified;
	}








	template<>
	struct FormatType<std::chrono::time_point<std::chrono::high_resolution_clock>>
	{
		static void Write(const std::chrono::time_point<std::chrono::high_resolution_clock>& t, Formater& formater) {
			if (!WriteTime(t, formater))
				formater.BufferParseUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};

	template<>
	struct FormatType<std::chrono::time_point<std::chrono::system_clock>>
	{
		static void Write(const std::chrono::time_point<std::chrono::system_clock>& t, Formater& formater) {
			if (!WriteTime(t, formater))
				formater.BufferParseUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};

	template<>
	struct FormatType<std::chrono::seconds>
	{
		static void Write(const std::chrono::seconds& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::seconds> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::seconds>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('s');
			}
		}
	};

	template<>
	struct FormatType<std::chrono::minutes>
	{
		static void Write(const std::chrono::minutes& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::minutes> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::minutes>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('m');
			}
		}
	};

	template<>
	struct FormatType<std::chrono::hours>
	{
		static void Write(const std::chrono::hours& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::hours> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::hours>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('h');
			}
		}
	};

	template<>
	struct FormatType<std::chrono::milliseconds>
	{
		static void Write(const std::chrono::milliseconds& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::milliseconds>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('m');
				formater.BufferPushBack('s');
			}
		}
	};

	template<>
	struct FormatType<std::chrono::microseconds>
	{
		static void Write(const std::chrono::microseconds& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::microseconds> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::microseconds>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('u');
				formater.BufferPushBack('s');
			}
		}
	};

	template<>
	struct FormatType<std::chrono::nanoseconds>
	{
		static void Write(const std::chrono::nanoseconds& t, Formater& formater) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> time(t);
			if (!WriteTime<std::chrono::high_resolution_clock, std::chrono::nanoseconds>(time, formater)) {
				formater.BufferParseUInt(t.count());
				formater.BufferPushBack('n');
				formater.BufferPushBack('s');
			}
		}
	};
}