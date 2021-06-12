#pragma once


#include "CPPTools/Formater/Formater.h"
#include <map>
#include <unordered_map>

namespace CPPTools::Fmt {
	template<typename T1, typename T2>
	struct FormatType<std::map<T1, T2>>
	{
		static void Write(const std::map<T1, T2>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			bool first = true;
			formater.PushBack('{');
			for(const std::pair<T1, T2>& pair : t) {
				if(!first)
					formater.WriteCharPt(nextElement);
				else
					first = false;
			
				FormatType<T1>::Write(pair.first, formater, data);
				formater.PushBack(':');
				FormatType<T2>::Write(pair.second, formater, data);
			}
			formater.PushBack('}');
		}
	};


	template<typename T1, typename T2>
	struct FormatType<std::unordered_map<T1, T2>>
	{
		static void Write(const std::unordered_map<T1, T2>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			bool first = true;
			formater.PushBack('{');
			for (const std::pair<T1, T2>& pair : t) {
				if (!first)
					formater.WriteCharPt(nextElement);
				else
					first = false;

				FormatType<T1>::Write(pair.first, formater, data);
				formater.PushBack(':');
				FormatType<T2>::Write(pair.second, formater, data);
			}
			formater.PushBack('}');
		}
	};
}