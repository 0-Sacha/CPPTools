#pragma once


#include "../BasicUnFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {


	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadInt(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign = context.BufferIsEqualForward('-');
		isValid = context.BufferIsADigit();

		while (context.BufferIsADigit())	{ res = res * 10 + (context.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadInt(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}






	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUInt(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = context.BufferIsADigit();

		while (context.BufferIsADigit())	{ res = res * 10 + (context.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = res; }
		return isValid;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUInt(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def) {
		return false;
	}

	template<typename T, typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, T& i, FormatDataType def, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}
}