#pragma once


#include "CPPTools/Formater/Formater.h"
#include <list>
#include <forward_list>
//#include <initializer_list>


namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::list<T>>
	{
		static void Write(const std::list<T>& t, Formater& formater) {
			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('{');

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

			formater.BufferPushBack('}');
		}
	};
	

	template<typename T>
	struct FormatType<std::forward_list<T>>
	{
		static void Write(const std::forward_list<T>& t, Formater& formater) {
			const char* nextElement = formater.GetFormatData().GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('{');

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

			formater.BufferPushBack('}');
		}
	};
}