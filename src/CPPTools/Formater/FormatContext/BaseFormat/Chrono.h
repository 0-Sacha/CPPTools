#pragma once

#include "../BasicFormatContextImpl.h"
#include <chrono>

namespace CPPTools::Fmt::ChronoDetail {

	template<typename Clock, typename Duration, typename FormatContext>
	void WriteTestTime(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		using namespace ::CPPTools::Fmt::FormatFunc;
		if (context.FormatNextIsSame("ns"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count());
		else if (context.FormatNextIsSame("us"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count());
		else if (context.FormatNextIsSame("ms"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count());
		else if (context.FormatNextIsSame("s"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count());
		else if (context.FormatNextIsSame("m"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count());
		else if (context.FormatNextIsSame("h"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count());
	}

	template<typename Clock, typename Duration, typename FormatContext>
	void WriteTestTimeMod(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		using namespace ::CPPTools::Fmt::FormatFunc;
		if (context.FormatNextIsSame("ns"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::nanoseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatNextIsSame("us"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatNextIsSame("ms"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::milliseconds>(value).time_since_epoch().count() % 1000, Detail::ShiftType::Right, 3, Detail::ShiftPrint::Zeros);
		else if (context.FormatNextIsSame("s"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::seconds>(value).time_since_epoch().count() % 60, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (context.FormatNextIsSame("m"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::minutes>(value).time_since_epoch().count() % 60, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
		else if (context.FormatNextIsSame("h"))
			FormatContextWriteUInt<uint32_t, FormatContext>(context, (uint32_t)std::chrono::time_point_cast<std::chrono::hours>(value).time_since_epoch().count() % 24, Detail::ShiftType::Right, 2, Detail::ShiftPrint::Zeros);
	}

	template<typename Clock, typename Duration, typename FormatContext>
	bool WriteTime(const std::chrono::time_point<Clock, Duration>& value, FormatContext& context) {
		bool isSpecified = false;
		if (context.FormatIsEqualForward(':')) {
			isSpecified = true;
			context.WriteUntilEndOfParameter('%', '#');
			while (!context.FormatIsEndOfParameter()) {
				if (context.FormatIsEqualForward('%'))			WriteTestTimeMod(value, context);
				else if (context.FormatIsEqualForward('#'))	WriteTestTime(value, context);
				else											context.FormatGoToEndOfParameter();
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
				context.BufferWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::time_point<std::chrono::system_clock>, FormatContext>
	{
		static void Write(const std::chrono::time_point<std::chrono::system_clock>& t, FormatContext& context) {
			if (!ChronoDetail::WriteTime(t, context))
				context.BufferWriteUInt((uint32_t)std::chrono::time_point_cast<std::chrono::microseconds>(t).time_since_epoch().count());
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::seconds, FormatContext>
	{
		static void Write(const std::chrono::seconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::seconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::seconds>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::minutes, FormatContext>
	{
		static void Write(const std::chrono::minutes& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::minutes> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::minutes>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('m');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::hours, FormatContext>
	{
		static void Write(const std::chrono::hours& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::hours> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::hours>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('h');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::milliseconds, FormatContext>
	{
		static void Write(const std::chrono::milliseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::milliseconds>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('m');
				context.BufferPushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::microseconds, FormatContext>
	{
		static void Write(const std::chrono::microseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::microseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::microseconds>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('u');
				context.BufferPushBack('s');
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<std::chrono::nanoseconds, FormatContext>
	{
		static void Write(const std::chrono::nanoseconds& t, FormatContext& context) {
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> time(t);
			if (!ChronoDetail::WriteTime<std::chrono::high_resolution_clock, std::chrono::nanoseconds>(time, context)) {
				context.BufferWriteUInt(t.count());
				context.BufferPushBack('n');
				context.BufferPushBack('s');
			}
		}
	};
}