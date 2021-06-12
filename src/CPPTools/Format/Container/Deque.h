#pragma once

#include "CPPTools/Formater/Formater.h"
#include <deque>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::deque<T>>
	{
		static void Write(const std::deque<T>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.BufferPushBack('[');

			bool first = true;
			for (const T& ele : t) {
				if (!first)			formater.BufferParseCharPt(nextElement);
				else				first = false;
				FormatType<T>::Write(ele, formater);
			}

			formater.BufferPushBack(']');
		}
	};

}