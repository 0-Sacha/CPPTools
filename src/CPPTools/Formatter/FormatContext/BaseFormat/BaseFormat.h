#pragma once

#include "../BasicFormatContextHelperFile.h"

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<FormatSpecifier, FormatContext> {
		static void Write(const FormatSpecifier& specifier, FormatContext& context) {
			if(specifier.ValueIsText)
				context.LittleFormat("{ '{}', '{}' }", specifier.Name, specifier.ValueAsText);
			else
				context.LittleFormat("{ '{}', '{}' }", specifier.Name, specifier.ValueAsNumber);
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
				if (t == true)	context.BufferWriteCharArray("True");
				else			context.BufferWriteCharArray("False");
			} else {
				if (t == true)	context.BufferPushBack('1');
				else			context.BufferPushBack('0');
			}
		}
	};

	// Int
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T> && std::is_signed_v<T>>, Detail::IsChar<T> >> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteInt(t);
		}
	};

	// UInt
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T> && !std::is_signed_v<T>>, Detail::IsChar<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteUInt(t);
		}
	};

	// Float
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfTrue<T, std::is_floating_point_v<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferWriteFloat(t);
		}
	};



	// Char type
	namespace Detail {
		template<typename CharType> struct ForwardAsChar {};
		template<typename CharType, std::size_t SIZE> struct ForwardAsCharArray {};
		template<typename CharType> struct ForwardAsCharPt {};
	}

	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsChar<T>, FormatContext> {
		inline static void Write(const T t, FormatContext& context) {
			context.BufferPushBack(t);
		}
	};

	template<typename T, std::size_t SIZE, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharArray<T, SIZE>, FormatContext> {
		static void Write(const T (&t)[SIZE], FormatContext& context) {
			FormatData& data = context.GetFormatData();
			
			std::size_t begin = context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0);
			std::size_t size = context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", SIZE - begin);

			if (data.BaseValue)	context.BufferPushBack('\'');
			context.BufferWriteCharPt(t + begin, size);
			if (data.BaseValue)	context.BufferPushBack('\'');
		}
	};

	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharPt<T>, FormatContext> {
		static void Write(const T* t, FormatContext& context) {
			FormatData& data = context.GetFormatData();

			if (data.BaseValue)										context.BufferPushBack('\'');
	
			std::size_t begin = context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0);
			std::size_t size = context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", std::numeric_limits<std::size_t>::max());

			if (size != std::numeric_limits<std::size_t>::max())	context.BufferWriteCharPt(t + begin, size);
			else													context.BufferWriteCharPt(t + begin);
	
			if (data.BaseValue)										context.BufferPushBack('\'');
		}
	};


	
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::IsChar<T>> {
		inline static void Write(const T t, FormatContext& context) {
			FormatType<Detail::ForwardAsChar<T>, FormatContext>::Write(t, context);
		}
	};
	template<std::size_t SIZE, typename T, typename FormatContext>
	struct FormatType<T [SIZE], FormatContext, Detail::IsCharArray<T, SIZE>> {
		static void Write(const T (&t)[SIZE], FormatContext& context) {
			FormatType<Detail::ForwardAsCharArray<T, SIZE>, FormatContext>::Write(t, context);
		}
	};
	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext, Detail::IsCharPt<T>> {
		static void Write(const T* const t, FormatContext& context) {
			FormatType<Detail::ForwardAsCharPt<T>, FormatContext>::Write(t, context);
		}
	};



	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename FormatContext>
	struct FormatType<T*, FormatContext, Detail::ForwardIfVoid<T*, Detail::IsCharPt<T>>>
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
	struct FormatType<T[SIZE], FormatContext, Detail::ForwardIfVoid<T[SIZE], Detail::IsCharArray<T, SIZE>>>
	{
		static void Write(T const (&t)[SIZE], FormatContext& context) {

			context.BufferWriteStringView(context.GetFormatData().GetValueAsTextOfSpecifierOr("begin", STDEnumerableUtility::DefaultBegin));

			std::size_t stride = context.GetStride();

			Detail::FormatSpecifierJoinSpliter join(context.GetFormatData().GetValueAsTextOfSpecifierOr("join", STDEnumerableUtility::DefaultJoin));

			bool first = true;
			std::size_t beginValue = context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0);
			const T* begin 	= &t[beginValue];
			const T* end  	= &begin[context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", SIZE - beginValue)];

			while(begin < end) {
				if (first)	first = false;
				else		{ join.Write(context, stride); }
				context.WriteType(*begin++);
			}

			context.BufferWriteStringView(context.GetFormatData().GetValueAsTextOfSpecifierOr("end", STDEnumerableUtility::DefaultEnd));
		}
	};
}