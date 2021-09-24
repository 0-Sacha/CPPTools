#pragma once

#include "CPPTools/Formatter/Formatter.h"

#include <tuple>
#include <utility>

namespace CPPTools::Fmt::TupleDetail {

	template<uint32_t N, typename ...Args> using NthTypeOf =
		typename std::tuple_element<N, std::tuple<Args...>>::type;

	template<uint32_t N, typename ...Args>
	constexpr NthTypeOf<N, Args...>& Get(Args&& ...args) {
		std::tuple<Args...> tuple(args...);
		return std::get<N>(tuple);
	}

	template<typename FormatContext>
	static void TupleFormatRec(FormatContext& context) { }

	template<typename T, typename FormatContext>
	static void TupleFormatRec(FormatContext& context, const T& t) {
		context.WriteType(t);
	}

	template<typename T, typename FormatContext, typename ...Args>
	static void TupleFormatRec(FormatContext& context, const T& t, Args&& ...args) {
		context.WriteType(t);
		context.BufferOut().PushBack(',');
		context.BufferOut().PushBack(' ');
		TupleFormatRec(context, args...);
	}

}

namespace CPPTools::Fmt {

	template<typename ...T, typename FormatContext>
	struct FormatType<std::tuple<T...>, FormatContext>
	{
		static void Write(const std::tuple<T...>& t, FormatContext& context) {
			context.BufferOut().PushBack('<');
			std::apply([&context](auto&&... args) { TupleDetail::TupleFormatRec(context, args...); }, t);
			context.BufferOut().PushBack('>');
		}
	};



	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::pair<T1, T2>, FormatContext>
	{
		static void Write(const std::pair<T1, T2>& t, FormatContext& context) {
			context.BufferOut().PushBack('<');
			context.WriteType(t.first);
			context.BufferOut().PushBack(':');
			context.WriteType(t.second);
			context.BufferOut().PushBack('>');
		}
	};
}
