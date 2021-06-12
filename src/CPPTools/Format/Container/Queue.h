#pragma once

#include "CPPTools/Formater/Formater.h"
#include <queue>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::queue<T>>
	{
		static void Write(const std::queue<T>& t, Formater& formater, const FormatData& data) {
		}
	};

}