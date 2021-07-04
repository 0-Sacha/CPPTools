#pragma once

#include "CPPTools/Formater/Formater.h""
#include <unordered_map>

#include "std_tuple.h"

namespace CPPTools::Fmt {

	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::unordered_map<T1, T2>, FormatContext>
	{
		static void Write(const std::unordered_map<T1, T2>& t, FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const std::pair<T1, T2>& element) {
				if (first)	first = false;
				else {
					context.BufferWriteCharType(nextElement);
					context.BufferAddSpaces(stride);
				}
				context.WriteType(element); });

			context.BufferPushBack(']');
		}
	};

	template<typename T1, typename T2, typename FormatContext>
	struct FormatType<std::unordered_multimap<T1, T2>, FormatContext>
	{
		static void Write(const std::unordered_multimap<T1, T2>& t, FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const std::pair<T1, T2>& element) {
				if (first)	first = false;
				else {
					context.BufferWriteCharType(nextElement);
					context.BufferAddSpaces(stride);
				}
				context.WriteType(element); });

			context.BufferPushBack(']');
		}
	};
}