#pragma once

#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt::Detail {

	/////---------- Const PT ----------/////

	template<typename T>
	struct RemoveConstPt
	{
		using type = T;
	};

	template<typename T>
	struct RemoveConstPt<T*>
	{
		using type = T*;
	};

	template<typename T>
	struct RemoveConstPt<const T*>
	{
		using type = T*;
	};

	template<typename T>
	using GetBaseType = typename RemoveConstPt<std::remove_cv_t<std::remove_reference_t<T>>>::type;



	
	template <typename T, typename ...Types>
	struct TMatchTo_ {
		using type = void;
	};

	template <typename T, typename Type>
	struct TMatchTo_<T, Type>
		: std::conditional<std::is_same_v<T, Type>, Type, void> {};

	template <typename T, typename Type, typename ...Types>
	struct TMatchTo_<T, Type, Types...> 
		: std::conditional<std::is_same_v<T, Type>, Type, typename TMatchTo_<T, Types...>::type> {};


	template <typename T, typename ...Types>
	using TMatchTo = typename TMatchTo_<T, Types...>::type;


	template <typename T>
	using IsChar = TMatchTo<T, char, wchar_t, char16_t, char32_t>;
	template <typename T, std::size_t SIZE>
	using IsCharArray = TMatchTo<T[SIZE], char[SIZE], wchar_t[SIZE], char16_t[SIZE], char32_t[SIZE]>;
	template <typename T>
	using IsCharPt = TMatchTo<T*, char*, wchar_t*, char16_t*, char32_t*>;



	template <typename Type, typename TestIsVoid>
	struct ForwardIfVoid_
		: std::conditional<std::is_same_v<TestIsVoid, void>, Type, void> {};

	template <typename Type, typename TestIsVoid>
	using ForwardIfVoid = typename ForwardIfVoid_<Type, TestIsVoid>::type;



	template <typename Type, bool Condition>
	using ForwardIfTrue = std::conditional_t<Condition, Type, void>;

}