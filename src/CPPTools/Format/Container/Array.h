#pragma once


#include "CPPTools/Formater/Formater.h"
#include <array>

namespace CPPTools::Fmt {

	template<size_t SIZE, typename T>
	struct FormatType<std::array<T, SIZE>>
	{
		static void Write(const std::array<T, SIZE>& t, Formater& formater, const FormatData& data) {

			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.PushBack('[');
			for (uint8_t i = 0; i < SIZE - 1; ++i) {
				FormatType<T>::Write(t[i], formater, data);
				formater.WriteCharPt(nextElement);
			}
			if (SIZE > 0)
				FormatType<T>::Write(t[SIZE - 1], formater, data);
			formater.PushBack(']');
		}
	};

}