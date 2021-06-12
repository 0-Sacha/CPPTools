#pragma once


#include "CPPTools/Formater/Formater.h"
#include <array>

namespace CPPTools::Fmt {

	template<size_t SIZE, typename T>
	struct FormatType<std::array<T, SIZE>>
	{
		static void Write(const std::array<T, SIZE>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('[');

			bool first = true;
			for (const T& ele : t) {
				if (!first)		formater.BufferParseCharPt(nextElement);
				else			first = false;

				FormatType<T>::Write(ele, formater);
			}

			formater.BufferPushBack(']');
		}
	};

}