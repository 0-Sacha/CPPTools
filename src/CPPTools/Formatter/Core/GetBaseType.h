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

}