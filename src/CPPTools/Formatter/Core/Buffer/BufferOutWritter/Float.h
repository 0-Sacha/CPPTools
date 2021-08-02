#pragma once

#include "../BufferOut.h"
#include "Integer.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteFloat(T i, FormatDataType nbDecimal) {
		BasicWriteInt<std::intmax_t>((std::intmax_t)i);
		PushBack('.');
		if (i < 0)	i = -i;
		i = i - (std::size_t)i;
		while (nbDecimal-- != 0) {
			i *= 10;
			PushBack((char)i + '0');
			i = i - (std::size_t)i;
		}
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteFloat(T i, FormatDataType nbDecimal, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		--shift;

		if (st == Detail::ShiftType::Right)
			while (shift-- > 0) PushBack(' ');

		if (i == 0)		PushBack('0');
		else {
			std::intmax_t i_int = (std::intmax_t)i;
			std::intmax_t i_ = i_int < 0 ? -i_int : i_int;
			std::int8_t nb = 0;
			while (i_ > 0)		{ i_ /= 10; ++nb; --shift; }
			shift -= nbDecimal;

			if (i_int < 0)		{ --shift;  PushBack('-'); i_int = -i_int; }
			Forward(nb - 1);
			while (i_int > 0) {
				PushReverse(i_int % 10 + '0');
				i_int /= 10;
			}
			Forward(nb + 1);

			PushBack('.');
			if (i < 0)
				i = -i;
			i = i - (std::size_t)i;
			while (nbDecimal-- != 0) {
				i *= 10;
				PushBack((char)i + '0');
				i = i - (std::size_t)i;
			}
		}

		if (st == Detail::ShiftType::Left)
			while (shift-- > 0) PushBack(' ');
	}

}