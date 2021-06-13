#pragma once


#include "CPPTools/Formater/Formater.h"
#include <map>
#include <unordered_map>

#include "Tuple.h"

namespace CPPTools::Fmt {
	template<typename T1, typename T2>
	struct FormatType<std::map<T1, T2>>
	{
		static void Write(const std::map<T1, T2>& t, Formater& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const std::pair<T1, T2>& element) {
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<std::pair<T1, T2>>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};


	template<typename T1, typename T2>
	struct FormatType<std::unordered_map<T1, T2>>
	{
		static void Write(const std::unordered_map<T1, T2>& t, Formater& formater) {
			formater.BufferPushBack('[');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const std::pair<T1, T2>& element) {
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<std::pair<T1, T2>>::Write(element, formater); });

			formater.BufferPushBack(']');
		}
	};
}