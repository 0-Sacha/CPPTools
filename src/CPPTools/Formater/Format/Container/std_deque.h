#pragma once

#include "CPPTools/Formater/Formater.h"
#include <deque>

namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::deque<T>, FormatContext>
	{
		static void Write(const std::deque<T>& t, FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			bool first = true;
			std::for_each_n(t.cbegin() + data.Begin, data.Size, [&](const T& element) { 
				if (first)	first = false;
				else {
					context.BufferWriteCharPt(nextElement);
					context.BufferAddSpaces(stride);
				}
				context.WriteType(element); });

			context.BufferPushBack(']');
		}
	};

}