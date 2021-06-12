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

			bool first = true;
			for (const T& ele : t) {
				if (!first)			formater.BufferParseCharPt(nextElement);
				else				first = false;
				FormatType<T>::Write(ele, formater);
			}

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

			bool first = true;
			for (const T& ele : t) {
				if (!first)			formater.BufferParseCharPt(nextElement);
				else				first = false;
				FormatType<T>::Write(ele, formater);
			}

			formater.BufferPushBack('}');
		}
	};
}