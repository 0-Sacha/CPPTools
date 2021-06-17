#pragma once

#include "CPPTools/Formater/BasicFormatContext.h"
#include <queue>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::queue<T>>
	{
		static void Write(const std::queue<T>& t, BasicFormatContext& formater, const FormatData& data) {
		}
	};

}