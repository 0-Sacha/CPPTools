#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<typename UnFormatContext::FormatDataType, UnFormatContext> {
		static bool Read(typename UnFormatContext::FormatDataType& t, UnFormatContext& context) {
			return false;
		}
	};


	template<typename UnFormatContext>
	struct UnFormatType<bool, UnFormatContext> {
		static bool Read(bool& t, UnFormatContext& context) {
			const auto& data = context.GetFormatData();

			if (!data.BaseValue) {
				switch (context.BufferGet())
				{
				case 'T':
				case 't':
					if (context.BufferNextIsSame("True"))	t = true;	return true;
				case 'F':
				case 'f':
					if (context.BufferNextIsSame("False"))	t = false;	return true;
				default:												return false;
				}
			}
			else {
				if (context.BufferIsEqualForward('0'))		{ t = false;	return true; }
				else if (context.BufferIsEqualForward('1'))	{ t = true;		return true; }
				else										return false;
			}
			return false;
		}
	};


	// Int
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T>&& std::is_signed_v<T>>, Detail::IsChar<T> >> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadInt(t);
		}
	};

	// UInt
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, Detail::ForwardIfVoid<Detail::ForwardIfTrue<T, std::is_integral_v<T> && !std::is_signed_v<T>>, Detail::IsChar<T>>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadUInt(t);
		}
	};

	// Float
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, Detail::ForwardIfTrue<T, std::is_floating_point_v<T>>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadFloat(t);
		}
	};



	// Char type
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, Detail::IsChar<T>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			context.BufferGetAndForward(t);
			return true;
		}
	};
	template<std::size_t SIZE, typename T, typename UnUnFormatContext>
	struct UnFormatType<T[SIZE], UnUnFormatContext, Detail::IsCharArray<T, SIZE> > {
		static bool Read(T(&t)[SIZE], UnUnFormatContext& context) {
			return false;
		}
	};
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T*, UnUnFormatContext, Detail::IsCharPt<T>> {
		static bool Read(T* const t, UnUnFormatContext& context) {
			return false;
		}
	};

	
	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename UnFormatContext>
	struct UnFormatType<T*, UnFormatContext, Detail::ForwardIfVoid<T*, Detail::IsCharPt<T>>>
	{
		static bool Read(T*& t, UnFormatContext& context) {
			return false;
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<void*, UnFormatContext>
	{
		static bool Read(void*& t, UnFormatContext& context) {
			return false;
		}
	};

	template<std::size_t SIZE, typename T, typename UnFormatContext>
	struct UnFormatType<T[SIZE], UnFormatContext, Detail::ForwardIfVoid<T[SIZE], Detail::IsCharArray<T, SIZE>>>
	{
		static bool Read(T (&t)[SIZE], UnFormatContext& context) {
			return false;
		}
	};
}