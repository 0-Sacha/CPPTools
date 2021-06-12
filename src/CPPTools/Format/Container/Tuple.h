#pragma once

#include "CPPTools/Formater/Formater.h"
#include <tuple>
#include <utility>

namespace CPPTools::Fmt {

	template<uint32_t N, typename ...Args> using NthTypeOf =
		typename std::tuple_element<N, std::tuple<Args...>>::type;

	template<uint32_t N, typename ...Args>
	constexpr NthTypeOf<N, Args...>& Get(Args&& ...args) {
		std::tuple<Args...> tuple(args...);
		return std::get<N>(tuple);
	}

	static void TupleFormatRec(Formater& formater) { }

	template<typename T>
	static void TupleFormatRec(Formater& formater, const T& t) {
		FormatType<T>::Write(t, formater);
	}

	template<typename T, typename ...Args>
	static void TupleFormatRec(Formater& formater, const T& t, Args&& ...args) {
		FormatType<T>::Write(t, formater);
		formater.BufferPushBack(',');
		formater.BufferPushBack(' ');
		TupleFormatRec(formater, args...);
	}

	template<typename ...T>
	struct FormatType<std::tuple<T...>>
	{
		static void Write(const std::tuple<T...>& t, Formater& formater, const FormatData& data) {
			formater.BufferPushBack('<');

			std::apply([&formater, &data](auto&&... args) { TupleFormatRec(formater, data, args...); }, t);

			formater.BufferPushBack('>');
		}
	};



	template<typename T1, typename T2>
	struct FormatType<std::pair<T1, T2>>
	{
		static void Write(const std::pair<T1, T2>& t, Formater& formater) {
			formater.BufferPushBack('<');
			FormatType<T1>::Write(t.first, formater);
			formater.BufferPushBack(':');
			FormatType<T1>::Write(t.second, formater);
			formater.BufferPushBack('>');
		}
	};
}
