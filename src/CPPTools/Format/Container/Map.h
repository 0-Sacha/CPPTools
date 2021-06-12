#pragma once


#include "CPPTools/Formater/Formater.h"
#include <map>
#include <unordered_map>

namespace CPPTools::Fmt {
	template<typename T1, typename T2>
	struct FormatType<std::map<T1, T2>>
	{
		static void Write(const std::map<T1, T2>& t, Formater& formater) {

			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('{');

			bool first = true;
			for(const std::pair<T1, T2>& pair : t) {
				if(!first)		formater.BufferParseCharPt(nextElement);
				else			first = false;
			
				FormatType<T1>::Write(pair.first, formater);
				formater.BufferPushBack(':');
				FormatType<T2>::Write(pair.second, formater);
			}

			formater.BufferPushBack('}');
		}
	};


	template<typename T1, typename T2>
	struct FormatType<std::unordered_map<T1, T2>>
	{
		static void Write(const std::unordered_map<T1, T2>& t, Formater& formater) {
			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('{');

			bool first = true;
			for (const std::pair<T1, T2>& pair : t) {
				if (!first)		formater.BufferParseCharPt(nextElement);
				else			first = false;

				FormatType<T1>::Write(pair.first, formater);
				formater.BufferPushBack(':');
				FormatType<T2>::Write(pair.second, formater);
			}

			formater.BufferPushBack('}');
		}
	};
}