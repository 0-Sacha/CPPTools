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


	template<typename Char>
	struct FormatType<Char, BasicFormatContext<Char>> {
		static void Write(const Char t, BasicFormatContext<Char>& context) {
			context.BufferPushBack(t);
		}
	};

#if 0
	template<typename FormatContext>
	struct FormatType<unsigned char, FormatContext> {
		static void Write(const unsigned char t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<short, FormatContext> {
		static void Write(const short t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<unsigned short, FormatContext> {
		static void Write(const unsigned short t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<int, FormatContext> {
		static void Write(const long t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<unsigned int, FormatContext> {
		static void Write(const unsigned int t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};
#endif

	template<typename FormatContext>
	struct FormatType<long, FormatContext> {
		static void Write(const long t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<unsigned long, FormatContext> {
		static void Write(const unsigned long t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<float, FormatContext> {
		static void Write(const float t, FormatContext& context) {
			context.BufferWriteFloat((double)t);
		}
	};

	template<typename FormatContext>
	struct FormatType<double, FormatContext> {
		static void Write(const double t, FormatContext& context) {
			context.BufferWriteFloat(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<long double, FormatContext> {
		static void Write(const long double t, FormatContext& context) {
			context.BufferWriteFloat(t);
		}
	};



	//------------------ STD Type ------------------//
	template<typename FormatContext>
	struct FormatType<std::int8_t, FormatContext> {
		static void Write(const std::int8_t t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::uint8_t, FormatContext> {
		static void Write(const std::uint8_t t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::int16_t, FormatContext> {
		static void Write(const std::int16_t t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::uint16_t, FormatContext> {
		static void Write(const std::uint16_t t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::int32_t, FormatContext> {
		static void Write(const std::int32_t t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::uint32_t, FormatContext> {
		static void Write(const std::uint32_t t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::int64_t, FormatContext> {
		static void Write(const std::int64_t t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	template<typename FormatContext>
	struct FormatType<std::uint64_t, FormatContext> {
		static void Write(const std::uint64_t t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};


	
	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext>
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


	template<std::size_t SIZE, typename T, typename Char>
	struct FormatType<T[SIZE], BasicFormatContext<Char>>
	{
		static void Write(T const (&t)[SIZE], BasicFormatContext<Char>& context) {

			context.BufferPushBack('[');

			FormatData& data = context.GetFormatData();

			data.SetMaxSize(SIZE);

			const Char* nextElement = data.ContainerPrintStyle == Detail::ContainerPrintStyle::CommaSpace ? "\n" : ", ";
			std::size_t stride		= data.ContainerPrintStyle == Detail::ContainerPrintStyle::CommaSpace ? context.GetStride() : 0;

			const T* begin = t + data.Begin;
			const T* end = begin + data.Size;

			bool first = true;
			while (begin < end) {
				if (first)	first = false;
				else {
					context.BufferWriteCharType(nextElement);
					context.BufferAddSpaces(stride);
				}
				FormatType<T, BasicFormatContext<Char>>::Write(*begin++, context);
			}

			context.BufferPushBack(']');
		}
	};


	//------------------ Specifier for Pointer/Array of Type ------------------//

	template<typename Char>
	struct FormatType<Char*, BasicFormatContext<Char>> {
		static void Write(const Char* const t, BasicFormatContext<Char>& context) {
			const FormatData& data = context.GetFormatData();

			if (data.BaseValue)						context.BufferPushBack('"');

			if (data.Size != -1)					context.BufferWriteCharType(t + data.Begin, data.Size);
			else									context.BufferWriteCharType(t + data.Begin);

			if (data.BaseValue)						context.BufferPushBack('"');
		}
	};

	template<size_t SIZE, typename Char>
	struct FormatType<Char[SIZE], BasicFormatContext<Char>> {
		static void Write(const Char(&t)[SIZE], BasicFormatContext<Char>& context) {
			FormatData& data = context.GetFormatData();
			data.SetMaxSize(SIZE);
			
			if (data.BaseValue)	context.BufferPushBack('"');
			context.BufferWriteCharType(t + data.Begin, data.Size);
			if (data.BaseValue)	context.BufferPushBack('"');
		}
	};
}