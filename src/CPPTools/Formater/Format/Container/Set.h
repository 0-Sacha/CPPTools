#pragma once

#include "CPPTools/Formater/Formater.h"
#include <set>
#include <unordered_set>

namespace CPPTools::Fmt {
	template<typename T, typename FormatContext>
	struct FormatType<std::set<T>, FormatContext>
	{
		static void Write(const std::set<T>& t, FormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
				if (first)	first = false;
				else {
					formater.BufferWriteCharType(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T, FormatContext>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};


	template<typename T, typename FormatContext>
	struct FormatType<std::unordered_set<T>, FormatContext>
	{
		static void Write(const std::unordered_set<T>& t, FormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
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