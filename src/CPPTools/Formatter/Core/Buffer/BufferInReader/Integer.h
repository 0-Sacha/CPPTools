#pragma once

#include "../BufferIn.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::FastReadInt(T& i) {
		T res = 0;

		bool sign = Base::IsEqualForward('-');
		if (!Base::IsADigit()) return false;

		while (Base::IsADigit())
			res = res * 10 + (Base::GetAndForward() - '0');

		i = sign ? -res : res;

		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::FastReadUInt(T& i) {
		T res = (T)0;

		if(!Base::IsADigit()) return false;

		while (Base::IsADigit())
			res = res * 10 + (Base::GetAndForward() - '0');

		i = res;

		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::FastReadFloat(T& i, FormatDataType nbDecimal) {
		typename Detail::ValuesDetail::FloatDetail<T>::IntType iInt;
		if(!FastReadInt(iInt)) return false;

		T res = 0;
		T dec = 0.1f;

		if (!Base::IsEqualForward('.')) return false;
		
		++nbDecimal;
		while (Base::IsADigit() && --nbDecimal != 0) {
			res += (Base::GetAndForward() - '0') * dec;
			dec *= 0.1f;
		}

		i = iInt + res;

		return true;
	}


	//---------------------------------------------//
	//---------------------------------------------//
	//---------------------------------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		T res = 0;

		SkipShiftBeginSpace(st, sp, shift);

		bool sign = Base::IsEqualForward('-'); 
		if (sign) --shift;
	
		if(!Base::IsADigit()) return false;

		while (Base::IsADigit()) {
			res = res * 10 + (Base::GetAndForward() - '0');
			--shift;
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;
		
		i = sign ? -res : res;
		
		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		T res = 0;

		SkipShiftBeginSpace(st, sp, shift);

		if(!Base::IsADigit()) return false;

		while (Base::IsADigit()) {
			res = res * 10 + (Base::GetAndForward() - '0');
			--shift;
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;

		i = res;

		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadFloat(T& i, FormatDataType nbDecimal, ShiftType st, FormatDataType shift, ShiftPrint sp) {

		typename Detail::ValuesDetail::FloatDetail<T>::IntType iInt = 0;

		SkipShiftBeginSpace(st, sp, shift);

		bool sign = Base::IsEqualForward('-');
		if (sign) --shift;

		if(!Base::IsADigit()) return false;

		while (Base::IsADigit()) {
			iInt = iInt * 10 + (Base::GetAndForward() - '0');
			--shift;
		}


		T res = 0;
		T dec = 0.1f;

		if(!Base::IsEqualForward('.')) return false;
		--shift;
			
		++nbDecimal;
		while (Base::IsADigit() && --nbDecimal != 0) {
			res += (Base::GetAndForward() - '0') * dec;
			dec *= 0.1f;
			--shift;
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;

		sign ? i = -iInt - res : i = iInt + res;

		return true;
	}

	//---------------------------------------------//
	//---------------------------------------------//
	//---------------------------------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsBin(T& i, FormatDataType digitSize, ShiftType st, FormatDataType shift, ShiftPrint sp, bool trueValue) {
		if (digitSize == Detail::DIGIT_SIZE_NOT_SPECIFIED) digitSize = sizeof(T) * 8;

		shift -= digitSize;
		if (trueValue) shift -= 2;

		SkipShiftBeginSpace(st, sp, shift);
		
		if (trueValue) {
			if(!Base::IsEqualForward('0')) return false;
			if(!Base::IsEqualForward('b')) return false;
		}

		T res = 0;

		while (Base::IsEqualTo('0', '1')) {
			res = res << 1;
			res += Base::Get() - '0';
			Base::Forward();
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;

		i = res;

		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsHex(T& i, FormatDataType digitSize, ShiftType st, FormatDataType shift, ShiftPrint sp, bool trueValue, Detail::PrintStyle valueDes) {
		if (digitSize == Detail::DIGIT_SIZE_NOT_SPECIFIED) digitSize = sizeof(T) * 2;

		shift -= digitSize;
		if (trueValue) shift -= 2;

		SkipShiftBeginSpace(st, sp, shift);

		if (trueValue) {
			if (!Base::IsEqualForward('0')) return false;
			if (!Base::IsEqualForward('x')) return false;
		}

		T res = 0;

		while (Base::IsADigit() || (Base::Get() >= 'A' && Base::Get() <= 'F') || (Base::Get() >= 'a' && Base::Get() <= 'f')) {
			res = res << 4;
			if (Base::IsADigit())								res += Base::Get() - '0';
			else if (Base::Get() >= 'A' && Base::Get() <= 'F')	res += Base::Get() - 'A' + 10;
			else if (Base::Get() >= 'a' && Base::Get() <= 'f')	res += Base::Get() - 'a' + 10;
			Base::Forward();
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;

		i = res;

		return true;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsOct(T& i, FormatDataType digitSize, ShiftType st, FormatDataType shift, ShiftPrint sp, bool trueValue) {
		if (digitSize == Detail::DIGIT_SIZE_NOT_SPECIFIED) digitSize = static_cast<FormatDataType>(std::ceil(static_cast<float>(sizeof(T) * 8) / 3));

		shift -= digitSize;
		if (trueValue) shift -= 2;

		SkipShiftBeginSpace(st, sp, shift);

		if (trueValue) {
			if (!Base::IsEqualForward('0')) return false;
			if (!Base::IsEqualForward('o')) return false;
		}

		T res = 0;

		while (Base::Get() >= '0' && Base::Get() <= '8') {
			res = res << 3;
			res += Base::Get() - '0';
			Base::Forward();
		}

		SkipShiftEnd(st, sp, shift);

		if (shift > 0)
			return false;

		i = res;

		return true;
	}

}