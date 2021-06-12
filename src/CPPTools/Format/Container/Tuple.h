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



	static void TupleFormatRec(Formater& formater, const FormatData& data) { }

	template<typename T>
	static void TupleFormatRec(Formater& formater, const FormatData& data, const T& t) {
		FormatType<T>::Write(t, formater, data);
	}

	template<typename T, typename ...Args>
	static void TupleFormatRec(Formater& formater, const FormatData& data, const T& t, Args&& ...args) {
		FormatType<T>::Write(t, formater, data);
		formater.PushBack(',');
		formater.PushBack(' ');
		TupleFormatRec(formater, data, args...);
	}

	template<typename ...T>
	struct FormatType<std::tuple<T...>>
	{
		static void Write(const std::tuple<T...>& t, Formater& formater, const FormatData& data) {

			formater.PushBack('<');

			std::apply([&formater, &data](auto&&... args) {
				TupleFormatRec(formater, data, args...);
			}, t);

			formater.PushBack('>');

		}
	};



	template<typename T1, typename T2>
	struct FormatType<std::pair<T1, T2>>
	{
		static void Write(const std::pair<T1, T2>& t, Formater& formater, const FormatData& data) {
			formater.LittleFormater("#{{:{2}}:{:{2}}}", t.first, t.second, data);
		}
	};
}
