#pragma once

#include "CPPTools/Formater/Formater.h"
#include <set>
#include <unordered_set>

namespace CPPTools::Fmt {
	template<typename T>
	struct FormatType<std::set<T>>
	{
		static void Write(const std::set<T>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.BufferPushBack('{');

			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater);
				formater.BufferParseCharPt(nextElement);
			}
			if (size > 0)	FormatType<T>::Write(t[size - 1], formater);

			formater.BufferPushBack('}');
		}
	};


	template<typename T>
	struct FormatType<std::unordered_set<T>>
	{
		static void Write(const std::unordered_set<T>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.BufferPushBack('{');
			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater);
				formater.BufferParseCharPt(nextElement);
			}
			if (size > 0)
				FormatType<T>::Write(t[size - 1], formater);
			formater.BufferPushBack('}');
		}
	};
}