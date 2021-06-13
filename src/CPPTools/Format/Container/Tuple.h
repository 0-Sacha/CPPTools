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

}

namespace CPPTools::Fmt {

	template<typename ...T>
	struct FormatType<std::tuple<T...>>
	{
		static void Write(const std::tuple<T...>& t, Formater& formater) {
			formater.BufferPushBack('<');

			std::apply([&formater](auto&&... args) { TupleDetail::TupleFormatRec(formater, args...); }, t);

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
			FormatType<T2>::Write(t.second, formater);
			formater.BufferPushBack('>');
		}
	};
}
