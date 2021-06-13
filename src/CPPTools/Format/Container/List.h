#pragma once


#include "CPPTools/Formater/Formater.h"
#include <list>
#include <forward_list>
//#include <initializer_list>


namespace CPPTools::Fmt {

	template<typename T>
	struct FormatType<std::list<T>>
	{
		static void Write(const std::list<T>& t, Formater& formater) {
			formater.BufferPushBack('{');

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

			formater.BufferPushBack('}');
		}
	};
	

	template<typename T>
	struct FormatType<std::forward_list<T>>
	{
		static void Write(const std::forward_list<T>& t, Formater& formater) {
			formater.BufferPushBack('{');

			FormatData& data = formater.GetFormatData();
			data.SetMaxSize(t.size());

			const char* nextElement = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride = data.ContainerPrintStyle == ContainerPrintStyle::NewLine ? formater.GetStride() : 0;

			bool first = true;
			std::for_each(t.cbegin(), t.cend(), [&](const T& element) {
				if (first)	first = false;
				else {
					formater.BufferParseCharPt(nextElement);
					formater.BufferAddSpaces(stride);
				}
				FormatType<T>::Write(element, formater); });

			formater.BufferPushBack('}');
		}
	};
}