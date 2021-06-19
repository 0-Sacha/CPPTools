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


	// Char type
	template<typename UnUnFormatContext>
	struct UnFormatType<char, UnUnFormatContext> {
		static void Read(const char t, UnUnFormatContext& context) {
			context.BufferGetAndForward(t);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char[SIZE], UnFormatContext> {
	};
	template<typename UnFormatContext>
	struct UnFormatType<char*, UnFormatContext> {
	};

	template<typename UnFormatContext>
	struct UnFormatType<wchar_t, UnFormatContext> {
		static void Read(const wchar_t t, UnFormatContext& context) {
			context.BufferGetAndForward(t);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<wchar_t[SIZE], UnFormatContext> {
	};
	template<typename UnFormatContext>
	struct UnFormatType<wchar_t*, UnFormatContext> {
	};

	template<typename UnFormatContext>
	struct UnFormatType<char16_t, UnFormatContext> {
		static void Read(const char16_t t, UnFormatContext& context) {
			context.BufferGetAndForward(t);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char16_t[SIZE], UnFormatContext> {
	};
	template<typename UnFormatContext>
	struct UnFormatType<char16_t*, UnFormatContext> {
	};

	template<typename UnFormatContext>
	struct UnFormatType<char32_t, UnFormatContext> {
		static void Read(const char32_t t, UnFormatContext& context) {
			context.BufferGetAndForward(t);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char32_t[SIZE], UnFormatContext> {
	};
	template<typename UnFormatContext>
	struct UnFormatType<char32_t*, UnFormatContext> {
	};

	
	//------------------ Pointer/Array of Type ------------------//

	template<typename T, typename UnFormatContext>
	struct UnFormatType<T*, UnFormatContext>
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
	struct UnFormatType<T[SIZE], UnFormatContext>
	{
		static bool Read(T (&t)[SIZE], UnFormatContext& context) {
			const FormatData& data = context.GetFormatData();
			return false;
		}
	};
}