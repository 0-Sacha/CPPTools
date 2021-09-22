#pragma once

#include "BasicBufferIn.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	class FormatterMemoryBufferIn : public BasicFormatterMemoryBufferIn<CharBuffer> {

	private:
		using Base = BasicFormatterMemoryBufferIn<CharBuffer>;

	public:
		using CharBufferType = CharBuffer;

	public:
		FormatterMemoryBufferIn(const std::basic_string_view<CharBuffer> format)
			: BasicFormatterMemoryBufferIn<CharBuffer>(format) {}

	public:
		// TODO
		template <typename ChildBuffer>
		inline void UpdateFromChlidBuffer(ChildBuffer& childBuffer) { SetBufferCurrentPos(childBuffer.GetBufferCurrentPos()); }

	public:
		template<typename T> bool FastReadInt	(T& i);
		template<typename T> bool FastReadUInt	(T& i);
		template<typename T> bool FastReadFloat	(T& i, FormatDataType floatPrecision = Detail::FLOAT_PRECISION_NOT_SPECIFIED);
		
		template<typename T> bool BasicReadInt		(T& i, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadUInt		(T& i, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadFloat	(T& i, FormatDataType floatPrecision = Detail::FLOAT_PRECISION_NOT_SPECIFIED, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space);
		
		template<typename T> bool BasicReadIntAsBin	(T& i, FormatDataType digitSize = Detail::DIGIT_SIZE_NOT_SPECIFIED, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space, bool trueValue = false);
		template<typename T> bool BasicReadIntAsHex	(T& i, FormatDataType digitSize = Detail::DIGIT_SIZE_NOT_SPECIFIED, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space, bool trueValue = false, Detail::PrintStyle valueDes = PrintStyle::Nothing);
		template<typename T> bool BasicReadIntAsOct	(T& i, FormatDataType digitSize = Detail::DIGIT_SIZE_NOT_SPECIFIED, ShiftType st = ShiftType::Nothing, FormatDataType shift = Detail::SHIFT_NOT_SPECIFIED, ShiftPrint sp = ShiftPrint::Space, bool trueValue = false);


	public:
		template<typename T, typename FormatDataCharType> bool ReadIntFormatData	(T& i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> bool ReadUIntFormatData	(T& i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> bool ReadFloatFormatData	(T& i, const FormatData<FormatDataCharType>& formatData);

	public:
		// Basic types
		template<typename T> bool BasicReadType(T& i) { return false; }

		inline bool BasicReadType(std::int8_t& i)	{ return FastReadInt(i); 	}
		inline bool BasicReadType(std::uint8_t& i)	{ return FastReadUInt(i); 	}
		inline bool BasicReadType(std::int16_t& i)	{ return FastReadInt(i);	}
		inline bool BasicReadType(std::uint16_t& i)	{ return FastReadUInt(i); 	}
		inline bool BasicReadType(std::int32_t& i)	{ return FastReadInt(i);	}
		inline bool BasicReadType(std::uint32_t& i)	{ return FastReadUInt(i); 	}
		inline bool BasicReadType(std::int64_t& i)	{ return FastReadInt(i);	}
		inline bool BasicReadType(std::uint64_t& i)	{ return FastReadUInt(i); 	}

		inline bool BasicReadType(float& i)			{ return FastReadFloat(i); }
		inline bool BasicReadType(double& i)		{ return FastReadFloat(i); }
		inline bool BasicReadType(long double& i)	{ return FastReadFloat(i); }

		inline bool BasicReadType(char& i)		{ i = Base::GetAndForward(); return true; }
		inline bool BasicReadType(wchar_t& i)	{ i = Base::GetAndForward(); return true; }
		inline bool BasicReadType(char16_t& i)	{ i = Base::GetAndForward(); return true; }
		inline bool BasicReadType(char32_t& i)	{ i = Base::GetAndForward(); return true; }

		template<std::size_t SIZE> inline bool BasicReadType(char(&i)[SIZE])		{ return true; }
		template<std::size_t SIZE> inline bool BasicReadType(wchar_t(&i)[SIZE])		{ return true; }
		template<std::size_t SIZE> inline bool BasicReadType(char16_t(&i)[SIZE])	{ return true; }
		template<std::size_t SIZE> inline bool BasicReadType(char32_t(&i)[SIZE])	{ return true; }

		template<typename CharType> inline bool BasicReadType(std::basic_string_view<CharType> i) { return true; }

		// Utils
	private:
		template<typename T>
		void SkipShiftBeginSpace(const Detail::ShiftType st, const Detail::ShiftPrint sp, T& shift) {
			if ((st == ShiftType::Right || st == ShiftType::Center) && sp == ShiftPrint::Space)
				while (Base::Get() == ' ') {
					Base::Forward();
					--shift;
				}
		}

		template<typename T>
		void SkipShiftEnd(const Detail::ShiftType st, const Detail::ShiftPrint sp, T& shift) {
			if (st == ShiftType::Left || st == ShiftType::Center)
				while (Base::Get() == ' ' && shift > 0) {
					Base::Forward();
					--shift;
				}
		}
	};

}