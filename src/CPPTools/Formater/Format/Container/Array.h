#pragma once


#include "CPPTools/Formater/Formater.h"
#include <array>

namespace CPPTools::Fmt {

	template<size_t SIZE, typename T, typename FormatContext>
	struct FormatType<std::array<T, SIZE>, FormatContext>
	{
		static void Write(const std::array<T, SIZE>& t, FormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(SIZE);

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each_n(t.cbegin() + data.Begin, data.Size, [&](const T& element) {
				if (first)	first = false;
				else {
					formater.BufferWriteCharType(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T, FormatContext>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};

}