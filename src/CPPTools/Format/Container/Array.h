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

			std::size_t stride = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? 0 : formater.GetStride();

			bool first = true;
			for (const T& ele : t) {
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T>::Write(ele, formater);
			}

			formater.BufferPushBack(']');
		}
	};

}