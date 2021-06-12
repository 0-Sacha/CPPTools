#pragma once


#include "CPPTools/Formater/Formater.h"
#include <list>
#include <forward_list>
//#include <initializer_list>


namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::list<T>>
	{
		static void Write(const std::list<T>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			bool first = true;
			formater.PushBack('{');
			for (const T& ele : t) {
				if (!first)
					formater.WriteCharPt(nextElement);
				else
					first = false;

				FormatType<T>::Write(ele, formater, data);
			}
			formater.PushBack('}');
		}
	};
	

	template<typename T>
	struct FormatType<std::forward_list<T>>
	{
		static void Write(const std::forward_list<T>& t, Formater& formater, const FormatData& data) {
			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			bool first = true;
			formater.PushBack('{');
			for (const T& ele : t) {
				if (!first)
					formater.WriteCharPt(nextElement);
				else
					first = false;

				FormatType<T>::Write(ele, formater, data);
			}
			formater.PushBack('}');
		}
	};
}