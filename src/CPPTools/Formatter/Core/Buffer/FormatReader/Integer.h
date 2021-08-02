#pragma once

#include "../Format.h"

namespace CPPTools::Fmt::Detail {
	template<typename CharFormat>
	template<typename T>
	bool FormatterMemoryFormat<CharFormat>::ReadInt(T& i) {
		T res = 0;
		bool sign		= BasicFormatterMemoryBufferIn<CharFormat>::IsEqualForward('-');
		bool isValid	= BasicFormatterMemoryBufferIn<CharFormat>::IsADigit();

		while (BasicFormatterMemoryBufferIn<CharFormat>::IsADigit()) { res = res * 10 + int(BasicFormatterMemoryBufferIn<CharFormat>::GetAndForward() - '0'); }
		if (isValid) { i = sign ? -res : res; }
		return isValid;
	}

	template<typename CharFormat>
	template<typename T>
	bool FormatterMemoryFormat<CharFormat>::ReadUInt(T& i) {
		T res = (T)0;
		bool isValid = BasicFormatterMemoryBufferIn<CharFormat>::IsADigit();

		while (BasicFormatterMemoryBufferIn<CharFormat>::IsADigit()) { res = res * 10 + (BasicFormatterMemoryBufferIn<CharFormat>::GetAndForward() - '0'); }

		if (isValid) { i = res; }
		return isValid;
	}
}