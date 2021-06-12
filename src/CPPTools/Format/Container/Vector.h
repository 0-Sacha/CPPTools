#pragma once


#include "CPPTools/Formater/Formater.h"
#include <vector>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::vector<T>>
	{
		static void Write(const std::vector<T>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.BufferPushBack('[');

			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater, data);
				formater.BufferParseCharPt(nextElement);
			}
			if (size > 0)		FormatType<T>::Write(t[size - 1], formater, data);

			formater.BufferPushBack(']');
		}
	};

}