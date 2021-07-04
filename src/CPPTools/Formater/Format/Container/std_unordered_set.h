#pragma once

#include "CPPTools/Formater/Formater.h"
#include <unordered_set>

namespace CPPTools::Fmt {
	template<typename T, typename FormatContext>
	struct FormatType<std::unordered_set<T>, FormatContext>
	{
		static void Write(const std::unordered_set<T>& t, FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
				if (first)	first = false;
				else {
					context.BufferWriteCharType(nextElement);
					context.BufferAddSpaces(stride);
				}
				context.WriteType(element); });

			context.BufferPushBack(']');
		}
	};

	template<typename T, typename FormatContext>
	struct FormatType<std::unordered_multiset<T>, FormatContext>
	{
		static void Write(const std::unordered_multiset<T>& t, FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
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