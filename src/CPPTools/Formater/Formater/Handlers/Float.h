#pragma once

#include "../Formater.h"
#include "Integer.h"

namespace CPPTools::Fmt::FormatFunc {

	template<typename T>
	void FormaterParseFloat(Formater& formater, T i, std::int16_t nbDecimal) {
		FormaterParseInt<std::intmax_t>(formater, (std::intmax_t)i);
		formater.BufferPushBack('.');
		if (i < 0)	i = -i;
		i = i - (std::size_t)i;
		while (nbDecimal-- != 0) {
			i *= 10;
			formater.BufferPushBack((char)i + '0');
			i = i - (std::size_t)i;
		}
	}

	template<typename T>
	void FormaterParseFloat(Formater& formater, T i, std::int16_t nbDecimal, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		--shift;

		if (st == Detail::ShiftType::Right)
			while (shift-- > 0) formater.BufferPushBack(' ');

		if (i == 0)		formater.BufferPushBack('0');
		else {
			std::intmax_t i_int = (std::intmax_t)i;
			std::intmax_t i_ = i_int < 0 ? -i_int : i_int;
			int nb = 0;
			while (i_ > 0)		{ i_ /= 10; ++nb; --shift; }
			shift -= nbDecimal;
			if (i_int < 0)		--shift;

			if (i_int < 0)		{ formater.BufferPushBack('-'); i_int = -i_int; }
			formater.BufferForward((std::size_t)(nb - 1));
			while (i_int > 0) {
				formater.BufferPushReverse(i_int % 10 + '0');
				i_int /= 10;
			}
			formater.BufferForward((std::size_t)(nb + 1));

			formater.BufferPushBack('.');
			if (i < 0)
				i = -i;
			i = i - (std::size_t)i;
			while (nbDecimal-- != 0) {
				i *= 10;
				formater.BufferPushBack((char)i + '0');
				i = i - (std::size_t)i;
			}
		}

		if (st == Detail::ShiftType::Left)
			while (shift-- > 0) formater.BufferPushBack(' ');
	}

}