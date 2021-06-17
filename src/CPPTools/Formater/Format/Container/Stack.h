#pragma once

#include "CPPTools/Formater/BasicFormatContext.h"
#include <stack>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::stack<T>>
	{
		static void Write(const std::stack<T>& t, BasicFormatContext& formater, const FormatData& data) {

		}
	};

}