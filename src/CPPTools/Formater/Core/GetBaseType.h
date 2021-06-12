#pragma once

namespace CPPTools::Fmt {

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
	using GetBaseType = typename RemoveConstPt<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type;

}