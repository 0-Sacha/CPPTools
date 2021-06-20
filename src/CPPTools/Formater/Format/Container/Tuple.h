#pragma once

#include "CPPTools/Formater/Formater.h"

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
	static void TupleFormatRec(FormatContext& formater) { }

	template<typename T, typename FormatContext>
	static void TupleFormatRec(FormatContext& formater, const T& t) {
		FormatType<T, FormatContext>::Write(t, formater);
	}

	template<typename T, typename FormatContext, typename ...Args>
	static void TupleFormatRec(FormatContext& formater, const T& t, Args&& ...args) {
		FormatType<T>::Write(t, formater);
		formater.BufferPushBack(',');
		formater.BufferPushBack(' ');
		TupleFormatRec(formater, args...);
	}

}

namespace CPPTools::Fmt {

	template<typename ...T, typename FormatContext>
	struct FormatType<std::tuple<T...>, FormatContext>
	{
		static void Write(const std::tuple<T...>& t, FormatContext& formater) {
			formater.BufferPushBack('<');
			std::apply([&formater](auto&&... args) { TupleDetail::TupleFormatRec(formater, args...); }, t);
			formater.BufferPushBack('>');
		}
	};



	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::pair<T1, T2>, FormatContext>
	{
		static void Write(const std::pair<T1, T2>& t, FormatContext& formater) {
			formater.BufferPushBack('<');
			FormatType<T1, FormatContext>::Write(t.first, formater);
			formater.BufferPushBack(':');
			FormatType<T2, FormatContext>::Write(t.second, formater);
			formater.BufferPushBack('>');
		}
	};
}
