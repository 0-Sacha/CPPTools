#pragma once

#include "CPPTools/Formater/Formater.h"
#include <queue>

namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::queue<T>, FormatContext>
	{
		static void Write(const std::queue<T>& t, FormatContext& context) {
		}
	};

}