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
				switch (context.BufferIn().Get())
				{
				case 'T':
				case 't':
					if (context.BufferIn().NextIsSame("True"))	t = true;	return true;
				case 'F':
				case 'f':
					if (context.BufferIn().NextIsSame("False"))	t = false;	return true;
				default:												return false;
				}
			}
			else {
				if (context.BufferIn().IsEqualForward('0'))			{ t = false;	return true; }
				else if (context.BufferIn().IsEqualForward('1'))	{ t = true;		return true; }
				else										return false;
			}
			return false;
		}
	};


	// Int
	template<typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsInt<T>, UnFormatContext> {
		static bool Read(T& t, UnFormatContext& context) {
			return context.BufferIn().ReadIntFormatData(t, context.GetFormatData());
		}
	};

	// UInt
	template<typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsUInt<T>, UnFormatContext> {
		static bool Read(T& t, UnFormatContext& context) {
			return context.BufferIn().ReadUIntFormatData(t, context.GetFormatData());
		}
	};

	// Float
	template<typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsFloat<T>, UnFormatContext> {
		static bool Read(T& t, UnFormatContext& context) {
			return context.BufferIn().ReadFloatFormatData(t, context.GetFormatData());
		}
	};

	// Char type
	template<typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsChar<T>, UnFormatContext> {
		static bool Read(T& t, UnFormatContext& context) {
			context.BufferIn().GetAndForward(t);
			return true;
		}
	};
	template<std::size_t SIZE, typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsCharArray<T, SIZE>, UnFormatContext> {
		static bool Read(T(&t)[SIZE], UnFormatContext& context) {
			return false;
		}
	};
	template<typename T, typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsCharPt<T>, UnFormatContext> {
		static bool Read(T* const t, UnFormatContext& context) {
			return false;
		}
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Int basic
	template<typename UnFormatContext>
	struct UnFormatType<std::int8_t, UnFormatContext> {
		static inline bool Read(std::int8_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::int8_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::int16_t, UnFormatContext> {
		static inline bool Read(std::int16_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::int16_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::int32_t, UnFormatContext> {
		static inline bool Read(std::int32_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::int32_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::int64_t, UnFormatContext> {
		static inline bool Read(std::int64_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::int64_t>, UnFormatContext>::Read(t, context);
		}
	};


	// UInt basic
	template<typename UnFormatContext>
	struct UnFormatType<std::uint8_t, UnFormatContext> {
		static inline bool Read(std::uint8_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::uint8_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::uint16_t, UnFormatContext> {
		static inline bool Read(std::uint16_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::uint16_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::uint32_t, UnFormatContext> {
		static inline bool Read(std::uint32_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::uint32_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<std::uint64_t, UnFormatContext> {
		static inline bool Read(std::uint64_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsInt<std::uint64_t>, UnFormatContext>::Read(t, context);
		}
	};


	// Float basic

	template<typename UnFormatContext>
	struct UnFormatType<float, UnFormatContext> {
		static inline bool Read(float &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsFloat<float>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<double, UnFormatContext> {
		static inline bool Read(double &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsFloat<double>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<long double, UnFormatContext> {
		static inline bool Read(long double &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsFloat<long double>, UnFormatContext>::Read(t, context);
		}
	};




	template<typename UnFormatContext>
	struct UnFormatType<char, UnFormatContext> {
		inline static bool Read(char &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsChar<char>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<wchar_t, UnFormatContext> {
		inline static bool Read(wchar_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsChar<wchar_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<char16_t, UnFormatContext> {
		inline static bool Read(char16_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsChar<char16_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<char32_t, UnFormatContext> {
		inline static bool Read(char32_t &t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsChar<char32_t>, UnFormatContext>::Read(t, context);
		}
	};

	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char[SIZE], UnFormatContext> {
		static bool Read(char(&t)[SIZE], UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharArray<char, SIZE>, UnFormatContext>::Read(t, context);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<wchar_t[SIZE], UnFormatContext> {
		static bool Read(wchar_t(&t)[SIZE], UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharArray<wchar_t, SIZE>, UnFormatContext>::Read(t, context);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char16_t[SIZE], UnFormatContext> {
		static bool Read(char16_t(&t)[SIZE], UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharArray<char16_t, SIZE>, UnFormatContext>::Read(t, context);
		}
	};
	template<std::size_t SIZE, typename UnFormatContext>
	struct UnFormatType<char32_t[SIZE], UnFormatContext> {
		static bool Read(char32_t(&t)[SIZE], UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharArray<char32_t, SIZE>, UnFormatContext>::Read(t, context);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<char*, UnFormatContext> {
		static bool Read(char* const t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharPt<char>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<wchar_t*, UnFormatContext> {
		static bool Read(wchar_t* const t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharPt<wchar_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<char16_t*, UnFormatContext> {
		static bool Read(char16_t* const t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharPt<char16_t>, UnFormatContext>::Read(t, context);
		}
	};
	template<typename UnFormatContext>
	struct UnFormatType<char32_t*, UnFormatContext> {
		static bool Read(char32_t* const t, UnFormatContext& context) {
			return UnFormatType<Detail::ForwardAsCharPt<char32_t>, UnFormatContext>::Read(t, context);
		}
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
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
			return false;
		}
	};
}