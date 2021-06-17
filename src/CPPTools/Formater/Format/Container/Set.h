#pragma once

#include "CPPTools/Formater/BasicFormatContext.h"
#include <set>
#include <unordered_set>

namespace CPPTools::Fmt {
	template<typename T>
	struct FormatType<std::set<T>>
	{
		static void Write(const std::set<T>& t, BasicFormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};


	template<typename T>
	struct FormatType<std::unordered_set<T>>
	{
		static void Write(const std::unordered_set<T>& t, BasicFormatContext& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
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