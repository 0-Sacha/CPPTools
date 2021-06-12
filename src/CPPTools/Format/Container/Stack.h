#pragma once

#include "CPPTools/Formater/Formater.h"
#include <stack>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::stack<T>>
	{
		static void Write(const std::stack<T>& t, Formater& formater, const FormatData& data) {

		}
	};

}