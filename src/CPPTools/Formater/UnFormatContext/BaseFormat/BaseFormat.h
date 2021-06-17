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


	template<typename Char>
	struct UnFormatType<Char, BasicUnFormatContext<Char>> {
		static bool Read(Char& t, BasicUnFormatContext<Char>& context) {
			t = context.BufferGetAndForward();
			return true;
		}
	};

#if 0
	template<typename UnFormatContext>
	struct UnFormatType<unsigned char, UnFormatContext> {
		static bool Read(unsigned char& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<short, UnFormatContext> {
		static bool Read(short& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<unsigned short, UnFormatContext> {
		static bool Read(unsigned short& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<int, UnFormatContext> {
		static bool Read(long& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<unsigned int, UnFormatContext> {
		static bool Read(unsigned int& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};
#endif

	template<typename UnFormatContext>
	struct UnFormatType<long, UnFormatContext> {
		static bool Read(long& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<unsigned long, UnFormatContext> {
		static bool Read(unsigned long& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<float, UnFormatContext> {
		static bool Read(float& t, UnFormatContext& context) {
			return context.BufferUnParseFloat(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<double, UnFormatContext> {
		static bool Read(double& t, UnFormatContext& context) {
			return context.BufferUnParseFloat(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<long double, UnFormatContext> {
		static bool Read(long double& t, UnFormatContext& context) {
			return context.BufferUnParseFloat(t);
		}
	};



	//------------------ STD Type ------------------//
	template<typename UnFormatContext>
	struct UnFormatType<std::int8_t, UnFormatContext> {
		static bool Read(std::int8_t& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::uint8_t, UnFormatContext> {
		static bool Read(std::uint8_t& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::int16_t, UnFormatContext> {
		static bool Read(std::int16_t& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::uint16_t, UnFormatContext> {
		static bool Read(std::uint16_t& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::int32_t, UnFormatContext> {
		static bool Read(std::int32_t& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::uint32_t, UnFormatContext> {
		static bool Read(std::uint32_t& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::int64_t, UnFormatContext> {
		static bool Read(std::int64_t& t, UnFormatContext& context) {
			return context.BufferUnParseInt(t);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<std::uint64_t, UnFormatContext> {
		static bool Read(std::uint64_t& t, UnFormatContext& context) {
			return context.BufferUnParseUInt(t);
		}
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



	//------------------ Specifier for Pointer/Array of Type ------------------//

	template<typename Char>
	struct UnFormatType<Char*, BasicFormatContext<Char>> {
		static bool Read(Char* t, BasicFormatContext<Char>& context) {
			return false;
		}
	};

	template<size_t SIZE, typename Char>
	struct UnFormatType<Char[SIZE], BasicFormatContext<Char>> {
		static bool Read(Char (&t)[SIZE], BasicFormatContext<Char>& context) {
			return false;
		}
	};
}