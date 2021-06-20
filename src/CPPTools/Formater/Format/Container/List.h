#pragma once


#include "CPPTools/Formater/Formater.h"
#include <list>
#include <forward_list>
//#include <initializer_list>


namespace CPPTools::Fmt {

	template<typename T, typename FormatContext>
	struct FormatType<std::list<T>, FormatContext>
	{
		static void Write(const std::list<T>& t, FormatContext& formater) {
			formater.BufferPushBack('{');

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

			formater.BufferPushBack('}');
		}
	};
	

	template<typename T, typename FormatContext>
	struct FormatType<std::forward_list<T>, FormatContext>
	{
		static void Write(const std::forward_list<T>& t, FormatContext& formater) {
			formater.BufferPushBack('{');

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

			formater.BufferPushBack('}');
		}
	};
}