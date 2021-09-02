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
			if (!context.GetFormatData().BaseValue) {
				if (t == true)	context.Print("True");
				else			context.Print("False");
			} else {
				if (t == true)	context.BufferOut().PushBack('1');
				else			context.BufferOut().PushBack('0');
			}
		}
	};

	// Int
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T> && std::is_signed_v<T>>, Detail::IsChar<T> >> {
		static void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteIntFormatData(t, context.GetFormatData());
		}
	};

	// UInt
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T> && !std::is_signed_v<T>>, Detail::IsChar<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteUIntFormatData(t, context.GetFormatData());
		}
	};

	// Float
	template<typename T, typename FormatContext>
	struct FormatType<T, FormatContext, Detail::ForwardIfTrue<T, std::is_floating_point_v<T>>> {
		static void Write(const T t, FormatContext& context) {
			context.BufferOut().WriteFloatFormatData(t, context.GetFormatData());
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
			context.BufferOut().PushBack(t);
		}
	};

	template<typename T, std::size_t SIZE, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharArray<T, SIZE>, FormatContext> {
		static void Write(const T (&t)[SIZE], FormatContext& context) {
			auto& data = context.GetFormatData();
			
			std::size_t begin	= context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0);
			std::size_t size	= context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", SIZE - begin);

			if (data.BaseValue)	context.BufferOut().PushBack('\'');
			context.BufferOut().WriteCharPt(t + begin, size);
			if (data.BaseValue)	context.BufferOut().PushBack('\'');
		}
	};

	template<typename T, typename FormatContext>
	struct FormatType<Detail::ForwardAsCharPt<T>, FormatContext> {
		static void Write(const T* t, FormatContext& context) {
			auto& data = context.GetFormatData();

			if (data.BaseValue)										context.BufferOut().PushBack('\'');
	
			std::size_t begin	= context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0);
			std::size_t size	= context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", (std::numeric_limits<std::size_t>::max)());

			if (size != (std::numeric_limits<std::size_t>::max)())	context.PrintCharPt(t + begin, size);
			else													context.PrintCharPt(t + begin);
	
			if (data.BaseValue)										context.BufferOut().PushBack('\'');
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

			context.BufferOut().WriteStringView(context.GetFormatData().GetValueAsTextOfSpecifierOr("begin", STDEnumerableUtility::DefaultBegin));

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

			context.BufferOut().WriteStringView(context.GetFormatData().GetValueAsTextOfSpecifierOr("end", STDEnumerableUtility::DefaultEnd));
		}
	};
}