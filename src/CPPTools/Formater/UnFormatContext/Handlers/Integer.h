#pragma once


#include "../BasicUnFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {


	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadInt(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign = context.BufferIsEqualForward('-');
		isValid = context.BufferIsADigit();

		while (context.BufferIsADigit())	{ res = res * 10 + (context.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadInt(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}






	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUInt(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = context.BufferIsADigit();

		while (context.BufferIsADigit())	{ res = res * 10 + (context.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = res; }
		return isValid;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUInt(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}
}