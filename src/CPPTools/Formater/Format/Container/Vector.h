#pragma once

#include "CPPTools/Formater/Formater.h"
#include <vector>

namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::vector<T>, FormatContext>
	{
		static void Write(const std::vector<T>& t, FormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

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