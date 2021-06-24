#pragma once

#include "../BasicFormatContext.h"

namespace CPPTools::Fmt {


	template<typename FormatContext>
	struct FormatType<FormatSpecifier, FormatContext> {
		static void Write(const FormatSpecifier& t, FormatContext& context) {
			context.BufferPushBack('{');
			context.BufferPushBack(' ');
			context.BufferPushBack(t.Type);
			context.BufferPushBack(';');
			context.BufferPushBack(' ');
			context.BufferWriteInt(t.Value);
			context.BufferPushBack(' ');
			context.BufferPushBack('}');
		}
	};
	template<typename FormatContext>
	struct FormatType<FormatData, FormatContext> {
		static void Write(const FormatData& t, FormatContext& context) {
			context.LittleFormat("{:C:red}", "Missing '{' or '}' because currently the format data is used as a parameter");
		}
	};



	// Bool
	template<typename FormatContext>
	struct FormatType<bool, FormatContext> {
		static void Write(const bool t, FormatContext& context) {
			if (context.GetFormatData().BaseValue) {
				if (t == true)	context.BufferWriteCharType("True");
				else			context.BufferWriteCharType("False");
			} else {
				if (t == true)	context.BufferPushBack('1');
				else			context.BufferPushBack('0');
			}
		}
	};

	// Int
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, ForwardIfVoid<ForwardIfTrue<T, std::is_integral_v<T> && std::is_signed_v<T>>, IsChar<T> >> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	// UInt
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, ForwardIfVoid<ForwardIfTrue<T, std::is_integral_v<T> && !std::is_signed_v<T>>, IsChar<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	// Float
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, ForwardIfTrue<T, std::is_floating_point_v<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteFloat(t);
		}
	};



	// Char type
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, IsChar<T>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferPushBack(t);
		}
	};
	template<std::size_t SIZE, typename T, typename FormatContext>
	struct FormatType<T [SIZE], FormatContext, IsCharArray<T, SIZE> > {
		static void Write(const T (&t)[SIZE], FormatContext& context) {
			FormatData& data = context.GetFormatData();
			data.SetMaxSize(SIZE);

			if (data.BaseValue)	context.BufferPushBack('"');
			context.BufferWriteCharType(t + data.Begin, data.Size);
			if (data.BaseValue)	context.BufferPushBack('"');
		}
	};
	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext, IsCharPt<T>> {
		static void Write(const T* const t, FormatContext& context) {
			const FormatData& data = context.GetFormatData();
	
			if (data.BaseValue)						context.BufferPushBack('"');
	
			if (data.Size != -1)					context.BufferWriteCharType(t + data.Begin, data.Size);
			else									context.BufferWriteCharType(t + data.Begin);
	
			if (data.BaseValue)						context.BufferPushBack('"');
		}
	};



	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext, ForwardIfVoid<T*, IsCharPt<T>>>
	{
		static void Write(const T* const t, FormatContext& context) {
			if(t == nullptr)	context.LittleFormat("{}", (void*)t);
			else				context.LittleFormat("{} -> {:{}}", (void*)t, *t, context.ForwardFormatData());
		}
	};

	template<typename FormatContext>
	struct FormatType<void*, FormatContext>
	{
		static void Write(const void* const t, FormatContext& context) {
			if (t == nullptr)	context.LittleFormat("nullptr");
			else				context.LittleFormat("0x{:x}", (std::size_t)t);
		}
	};

	template<std::size_t SIZE, typename T, typename FormatContext>
	struct FormatType<T[SIZE], FormatContext, ForwardIfVoid<T[SIZE], IsCharArray<T, SIZE>>>
	{
		static void Write(T const (&t)[SIZE], FormatContext& context) {
			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();

			data.SetMaxSize(SIZE);

			const char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::NewLine ? context.GetStride() : 0;

			const T* begin = t + data.Begin;
			const T* end = begin + data.Size;

			bool first = true;
			while (begin < end) {
				if (first)	first = false;
				else {
					context.BufferWriteCharType(nextElement);
					context.BufferAddSpaces(stride);
				}
				FormatType<T, FormatContext>::Write(*begin++, context);
			}

			context.BufferPushBack(']');
		}
	};
}