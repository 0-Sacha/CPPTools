#pragma once

#include "../BasicFormatContext.h"
#include <chrono>

namespace CPPTools::Fmt::ChronoDetail {

	template<typename Clock, typename Duration, typename FormatContext>
	void WriteTestTime(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		if (context.FormatStr().NextIsSame("ns"))		context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count());
		else if (context.FormatStr().NextIsSame("us"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count());
		else if (context.FormatStr().NextIsSame("ms"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count());
		else if (context.FormatStr().NextIsSame("s"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count());
		else if (context.FormatStr().NextIsSame("m"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count());
		else if (context.FormatStr().NextIsSame("h"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count());
	}

	template<typename Clock, typename Duration, typename FormatContext>
	void WriteTestTimeMod(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		if (context.FormatStr().NextIsSame("ns"))		context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count()	% 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatStr().NextIsSame("us"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count()	% 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatStr().NextIsSame("ms"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count()	% 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatStr().NextIsSame("s"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count()		% 60,	Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (context.FormatStr().NextIsSame("m"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count()		% 60,	Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (context.FormatStr().NextIsSame("h"))	context.BufferOut().BasicWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count()			% 24,	Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
	}

	template<typename Clock, typename Duration, typename FormatContext>
	bool WriteTime(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		bool isSpecified = false;
		if (context.FormatStr().IsEqualForward(':')) {
			isSpecified = true;
			context.WriteUntilEndOfParameter('%', '#');
			while (!context.FormatStr().IsEndOfParameter()) {
				if (context.FormatStr().IsEqualForward('%'))		WriteTestTimeMod(value, context);
				else if (context.FormatStr().IsEqualForward('#'))	WriteTestTime(value, context);
				else												context.FormatStr().GoToEndOfParameter();
				context.WriteUntilEndOfParameter('%', '#');
			}
		}
		return isSpecified;
	}

}


namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<std::chrono::time_point<std::chrono::high_resolution_clock>, FormatContext>
	{
		static void Write(const std::chrono::time_point<std::chrono::high_resolution_clock>& t, FormatContext& context) {
			if (!ChronoDetail::WriteTime(t, context))
				context.BufferOut().BasicWriteType((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};

#ifdef CPPTOOLS_COMPILER_MSVC
	template<typename FormatContext>
	struct FormatType<std::chrono::time_point<std::chrono::system_clock>, FormatContext>
	{
		static void Write(const std::chrono::time_point<std::chrono::system_clock>& t, FormatContext& context) {
			if (!ChronoDetail::WriteTime(t, context))
				context.BufferOut().BasicWriteType((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};
#endif
	template<typename FormatContext>
	struct FormatType<std::chrono::seconds, FormatContext>
	{
		static void Write(const std::chrono::seconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::seconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::seconds>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::minutes, FormatContext>
	{
		static void Write(const std::chrono::minutes& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::minutes> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::minutes>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('m');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::hours, FormatContext>
	{
		static void Write(const std::chrono::hours& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::hours> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::hours>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('h');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::milliseconds, FormatContext>
	{
		static void Write(const std::chrono::milliseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::milliseconds>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('m');
				context.BufferOut().PushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::microseconds, FormatContext>
	{
		static void Write(const std::chrono::microseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::microseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::microseconds>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('u');
				context.BufferOut().PushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::nanoseconds, FormatContext>
	{
		static void Write(const std::chrono::nanoseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::nanoseconds>(time, context)) {
				context.BufferOut().BasicWriteType(t.count());
				context.BufferOut().PushBack('n');
				context.BufferOut().PushBack('s');
			}
		}
	};
}