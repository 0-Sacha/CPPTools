#pragma once

#include "CPPTools/Formater/Formater.h"
#include <deque>

namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::deque<T>>
	{
		static void Write(const std::deque<T>& t, Formater& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each_n(t.cbegin() + data.Begin, data.Size, [&](const T& element) { 
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};

}