#pragma once

#include "../BasicFormatContext.h"

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<typename FormatContext::FormatSpecifierType, FormatContext> {
		static void Write(const typename FormatContext::FormatSpecifierType& specifier, FormatContext& context) {
			if(specifier.ValueIsText)
				context.LittleFormat("{ '{}', '{}' }", specifier.Name, specifier.ValueAsText);
			else
				context.LittleFormat("{ '{}', '{}' }", specifier.Name, specifier.ValueAsNumber);
		}
	};

	template<typename FormatContext>
	struct FormatType<typename FormatContext::FormatDataType, FormatContext> {
		static void Write(const typename FormatContext::FormatDataType& t, FormatContext& context) {
			context.LittleFormat("{:C:red}", "Missing '{' or '}' because currently the format data is used as a parameter");
		}
	};



	// Bool
	template<typename FormatContext>
	struct FormatType<bool, FormatContext> {
		static void Write(const bool t, FormatContext& context) {
			if (!context.GetFormatData().TrueValue) {
				if (t == true)	context.Print("True");
				else			context.Print("False");
			} else {
				if (t == true)	context.BufferOut().PushBack('1');
				else			context.BufferOut().PushBack('0');
			}
		}
	};

	// Int basic
	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsInt<T>, FormatContext> {
		static inline void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteIntFormatData(t, context.GetFormatData());
		}
	};

	// UInt basic
	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsUInt<T>, FormatContext> {
		static inline void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteUIntFormatData(t, context.GetFormatData());
		}
	};

	// Float basic
	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsFloat<T>, FormatContext> {
		static void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteFloatFormatData(t, context.GetFormatData());
		}
	};

	// Char type
	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsChar<T>, FormatContext> {
		inline static void Write(const T t, FormatContext& context) {
			context.BufferOut().PushBack(t);
		}
	};

	template<typename T, std::size_t SIZE, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharArray<T, SIZE>, FormatContext> {
		static void Write(const T(&t)[SIZE], FormatContext& context) {
			auto& data = context.GetFormatData();

			auto begin = context.GetFormatData().GetSpecifierAsNumber("begin", 0);
			auto size = context.GetFormatData().GetSpecifierAsNumber("size", SIZE - begin);

			if (data.TrueValue)	context.BufferOut().PushBack('\"');
			context.BufferOut().WriteCharPt(t + begin, size);
			if (data.TrueValue)	context.BufferOut().PushBack('\"');
		}
	};

	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharPt<T>, FormatContext> {
		static void Write(const T* t, FormatContext& context) {
			auto& data = context.GetFormatData();

			if (data.TrueValue)										context.BufferOut().PushBack('\"');

			auto begin = context.GetFormatData().GetSpecifierAsNumber("begin", 0);
			auto size = context.GetFormatData().GetSpecifierAsNumber("size", Detail::FORMAT_DATA_NOT_SPECIFIED);

			if (size != Detail::FORMAT_DATA_NOT_SPECIFIED)	context.PrintCharPt(t + begin, size);
			else											context.PrintCharPt(t + begin);

			if (data.TrueValue)										context.BufferOut().PushBack('\"');
		}
	};


	// Int basic
	template<typename FormatContext>
	struct FormatType<std::int8_t, FormatContext> {
		static inline void Write(const std::int8_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsInt<std::int8_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::int16_t, FormatContext> {
		static inline void Write(const std::int16_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsInt<std::int16_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::int32_t, FormatContext> {
		static inline void Write(const std::int32_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsInt<std::int32_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::int64_t, FormatContext> {
		static inline void Write(const std::int64_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsInt<std::int64_t>, FormatContext>::Write(t, context);
		}
	};


	// UInt basic
	template<typename FormatContext>
	struct FormatType<std::uint8_t, FormatContext> {
		static inline void Write(const std::uint8_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsUInt<std::uint8_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::uint16_t, FormatContext> {
		static inline void Write(const std::uint16_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsUInt<std::uint16_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::uint32_t, FormatContext> {
		static inline void Write(const std::uint32_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsUInt<std::uint32_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<std::uint64_t, FormatContext> {
		static inline void Write(const std::uint64_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsUInt<std::uint64_t>, FormatContext>::Write(t, context);
		}
	};


	// Float basic

	template<typename FormatContext>
	struct FormatType<float, FormatContext> {
		static inline void Write(const float t, FormatContext& context) {
			FormatType<Detail::ForwardAsFloat<float>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<double, FormatContext> {
		static inline void Write(const double t, FormatContext& context) {
			FormatType<Detail::ForwardAsFloat<double>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<long double, FormatContext> {
		static inline void Write(const long double t, FormatContext& context) {
			FormatType<Detail::ForwardAsFloat<long double>, FormatContext>::Write(t, context);
		}
	};



	
	template<typename FormatContext>
	struct FormatType<char, FormatContext> {
		inline static void Write(const char t, FormatContext& context) {
			FormatType<Detail::ForwardAsChar<char>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<wchar_t, FormatContext> {
		inline static void Write(const wchar_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsChar<wchar_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<char16_t, FormatContext> {
		inline static void Write(const char16_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsChar<char16_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<char32_t, FormatContext> {
		inline static void Write(const char32_t t, FormatContext& context) {
			FormatType<Detail::ForwardAsChar<char32_t>, FormatContext>::Write(t, context);
		}
	};

	template<std::size_t SIZE, typename FormatContext>
	struct FormatType<char [SIZE], FormatContext> {
		static void Write(const char (&t)[SIZE], FormatContext& context) {
			FormatType<Detail::ForwardAsCharArray<char, SIZE>, FormatContext>::Write(t, context);
		}
	};
	template<std::size_t SIZE, typename FormatContext>
	struct FormatType<wchar_t[SIZE], FormatContext> {
		static void Write(const wchar_t(&t)[SIZE], FormatContext& context) {
			FormatType<Detail::ForwardAsCharArray<wchar_t, SIZE>, FormatContext>::Write(t, context);
		}
	};
	template<std::size_t SIZE, typename FormatContext>
	struct FormatType<char16_t[SIZE], FormatContext> {
		static void Write(const char16_t(&t)[SIZE], FormatContext& context) {
			FormatType<Detail::ForwardAsCharArray<char16_t, SIZE>, FormatContext>::Write(t, context);
		}
	};
	template<std::size_t SIZE, typename FormatContext>
	struct FormatType<char32_t[SIZE], FormatContext> {
		static void Write(const char32_t(&t)[SIZE], FormatContext& context) {
			FormatType<Detail::ForwardAsCharArray<char32_t, SIZE>, FormatContext>::Write(t, context);
		}
	};

	template<typename FormatContext>
	struct FormatType<char*, FormatContext> {
		static void Write(const char* const t, FormatContext& context) {
			FormatType<Detail::ForwardAsCharPt<char>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<wchar_t*, FormatContext> {
		static void Write(const wchar_t* const t, FormatContext& context) {
			FormatType<Detail::ForwardAsCharPt<wchar_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<char16_t*, FormatContext> {
		static void Write(const char16_t* const t, FormatContext& context) {
			FormatType<Detail::ForwardAsCharPt<char16_t>, FormatContext>::Write(t, context);
		}
	};
	template<typename FormatContext>
	struct FormatType<char32_t*, FormatContext> {
		static void Write(const char32_t* const t, FormatContext& context) {
			FormatType<Detail::ForwardAsCharPt<char32_t>, FormatContext>::Write(t, context);
		}
	};



	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext>
	{
		static void Write(const T* const t, FormatContext& context) {

			if (t == nullptr) {
				context.LittleFormat("{}", (void*)t); return;
			}

			auto size = context.GetFormatData().GetSpecifierAsNumber("size", Detail::FORMAT_DATA_NOT_SPECIFIED);

			if(size == Detail::FORMAT_DATA_NOT_SPECIFIED) {
				context.LittleFormat("{} -> {:{}}", (void*)t, *t, context.ForwardFormatData());
				return;
			}


			context.BufferOut().WriteStringView(context.GetFormatData().GetSpecifierAsText("begin", STDEnumerableUtility::DefaultBegin));

			std::size_t stride = context.GetStride();

			Detail::FormatSpecifierJoinSpliter join(context.GetFormatData().GetSpecifierAsText("join", STDEnumerableUtility::DefaultJoin));

			auto beginValue = context.GetFormatData().GetSpecifierAsNumber("begin", 0);

			bool first = true;
			const T* begin	= t + beginValue;
			const T* end	= begin + size;

			while (begin < end) {
				if (first)	first = false;
				else		join.Write(context, stride);
				context.WriteType(*begin++);
			}

			context.BufferOut().WriteStringView(context.GetFormatData().GetSpecifierAsText("end", STDEnumerableUtility::DefaultEnd));
		}
	};

	template<typename FormatContext>
	struct FormatType<void*, FormatContext>
	{
		static void Write(const void* const t, FormatContext& context) {
			if (t == nullptr)	context.LittleFormat("nullptr");
			else				context.LittleFormat("{:X,=,U}", (std::size_t)t);
		}
	};

	template<std::size_t SIZE, typename T, typename FormatContext>
	struct FormatType<T[SIZE], FormatContext>
	{
		static void Write(T const (&t)[SIZE], FormatContext& context) {

			context.BufferOut().WriteStringView(context.GetFormatData().GetSpecifierAsText("begin", STDEnumerableUtility::DefaultBegin));

			std::size_t stride = context.GetStride();

			Detail::FormatSpecifierJoinSpliter join(context.GetFormatData().GetSpecifierAsText("join", STDEnumerableUtility::DefaultJoin));

			bool first = true;
			std::size_t beginValue = context.GetFormatData().GetSpecifierAsNumber("begin", 0);
			const T* begin 	= t + beginValue;
			const T* end  	= begin + context.GetFormatData().GetSpecifierAsNumber("size", SIZE - beginValue);

			while(begin < end) {
				if (first)	first = false;
				else		join.Write(context, stride);
				context.WriteType(*begin++);
			}

			context.BufferOut().WriteStringView(context.GetFormatData().GetSpecifierAsText("end", STDEnumerableUtility::DefaultEnd));
		}
	};
}