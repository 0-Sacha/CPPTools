#pragma once

#include "CPPTools/Formater/Formater.h"
#include <deque>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::deque<T>>
	{
		static void Write(const std::deque<T>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.PushBack('[');
			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater, data);
				formater.WriteCharPt(nextElement);
			}
			if (size > 0)
				FormatType<T>::Write(t[size - 1], formater, data);
			formater.PushBack(']');
		}
	};

}