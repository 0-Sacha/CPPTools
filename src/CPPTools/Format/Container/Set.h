#pragma once

#include "CPPTools/Formater/Formater.h"
#include <set>
#include <unordered_set>

namespace CPPTools::Fmt {
	template<typename T>
	struct FormatType<std::set<T>>
	{
		static void Write(const std::set<T>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.PushBack('{');
			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater, data);
				formater.WriteCharPt(nextElement);
			}
			if (size > 0)
				FormatType<T>::Write(t[size - 1], formater, data);
			formater.PushBack('}');
		}
	};


	template<typename T>
	struct FormatType<std::unordered_set<T>>
	{
		static void Write(const std::unordered_set<T>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";
			size_t size = t.size();

			formater.PushBack('{');
			for (uint8_t i = 0; i < size - 1; ++i) {
				FormatType<T>::Write(t[i], formater, data);
				formater.WriteCharPt(nextElement);
			}
			if (size > 0)
				FormatType<T>::Write(t[size - 1], formater, data);
			formater.PushBack('}');
		}
	};
}