#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<FormatData, UnFormatContext> {
		static bool Read(FormatData& t, UnFormatContext& context) {
			return false;
		}
	};


	template<typename UnFormatContext>
	struct UnFormatType<bool, UnFormatContext> {
		static bool Read(bool& t, UnFormatContext& context) {
			const FormatData& data = context.GetFormatData();

			if (data.BaseValue) {
				switch (context.BufferGet())
				{
				case 'T':
				case 't':
					if (context.BufferNextIsSame("True"))	t = true;
					return true;
				case 'F':
				case 'f':
					if (context.BufferNextIsSame("False"))	t = false;
					return true;
				}
				return false;
			}
			else {
				if (context.BufferIsEqualForward('0'))		t = false;
				else if (context.BufferIsEqualForward('1'))	t = true;
					else
						return false;
			}
			return true;
		}
	};


	// Int
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, ForwardIfVoid<ForwardIfTrue<T, std::is_integral_v<T>&& std::is_signed_v<T>>, IsChar<T> >> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadInt(t);
		}
	};

	// UInt
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, ForwardIfVoid<ForwardIfTrue<T, std::is_integral_v<T> && !std::is_signed_v<T>>, IsChar<T>>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadUInt(t);
		}
	};

	// Float
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, ForwardIfTrue<T, std::is_floating_point_v<T>>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			return context.BufferReadFloat(t);
		}
	};



	// Char type
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T, UnUnFormatContext, IsChar<T>> {
		static bool Read(T& t, UnUnFormatContext& context) {
			context.BufferGetAndForward(t);
			return true;
		}
	};
	template<std::size_t SIZE, typename T, typename UnUnFormatContext>
	struct UnFormatType<T[SIZE], UnUnFormatContext, IsCharArray<T, SIZE> > {
		static bool Read(T(&t)[SIZE], UnUnFormatContext& context) {
			return false;
		}
	};
	template<typename T, typename UnUnFormatContext>
	struct UnFormatType<T*, UnUnFormatContext, IsCharPt<T>> {
		static bool Read(T* const t, UnUnFormatContext& context) {
			return false;
		}
	};

	
	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename UnFormatContext>
	struct UnFormatType<T*, UnFormatContext, ForwardIfVoid<T*, IsCharPt<T>>>
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
	struct UnFormatType<T[SIZE], UnFormatContext, ForwardIfVoid<T[SIZE], IsCharArray<T, SIZE>>>
	{
		static bool Read(T (&t)[SIZE], UnFormatContext& context) {
			const FormatData& data = context.GetFormatData();
			return false;
		}
	};
}