#pragma once

#include "../BufferIn.h"

namespace CPPTools::Fmt::Detail {

	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadInt(T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign	= BasicFormatterMemoryBufferIn<CharBuffer>::IsEqualForward('-');
		isValid = BasicFormatterMemoryBufferIn<CharBuffer>::IsADigit();

		while (BasicFormatterMemoryBufferIn<CharBuffer>::IsADigit())	{ res = res * 10 + (BasicFormatterMemoryBufferIn<CharBuffer>::GetAndForward() - '0'); }

		if (isValid)						{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Bin --------------------//
	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsBin(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsBin(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Hex --------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsHex(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsHex(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsOct(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadIntAsOct(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}






	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUInt(T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = BasicFormatterMemoryBufferIn<CharBuffer>::IsADigit();

		while (BasicFormatterMemoryBufferIn<CharBuffer>::IsADigit())	{ res = res * 10 + (BasicFormatterMemoryBufferIn<CharBuffer>::GetAndForward() - '0'); }

		if (isValid)						{ i = res; }
		return isValid;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsBin(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsBin(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Hex --------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsHex(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsHex(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsOct(T& i, FormatDataType def) {
		return false;
	}

	template<typename CharBuffer>
	template<typename T>
	bool FormatterMemoryBufferIn<CharBuffer>::BasicReadUIntAsOct(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		return false;
	}
}