#pragma once

#include "../Formater.h"

namespace CPPTools::Fmt {

	template<typename T>
	bool Formater::FormatReadInt(T& i) {
		T res = 0;
		bool sign = false, isValid = false;
		char c = *m_SubFormat++;
		if (c == '-') {
			c = *m_SubFormat++;
			sign = true;
		}
		if (c >= '0' && c <= '9')
			isValid = true;
		while (c >= '0' && c <= '9') {
			res = res * 10 + c - '0';
			c = *m_SubFormat++;
		}
		--m_SubFormat;
		if (isValid)
			i = sign ? -res : res;
		return isValid;
	}

	template<typename T>
	bool Formater::FormatReadUInt(T& i) {
		T res = (T)0;
		bool isValid = false;
		char c = *m_SubFormat++;
		if (c >= '0' && c <= '9')
			isValid = true;
		while (c >= '0' && c <= '9') {
			res = res * 10 + c - '0';
			c = *m_SubFormat++;
		}
		--m_SubFormat;
		if (isValid)
			i = res;
		return isValid;
	}
}