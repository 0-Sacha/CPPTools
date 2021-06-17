#pragma once


#include "../BasicUnFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {


	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadInt(BasicUnFormatContext<Char>& formater, T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign = formater.BufferIsEqualForward('-');
		isValid = formater.BufferIsADigit();

		while (formater.BufferIsADigit())	{ res = res * 10 + (formater.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadInt(BasicUnFormatContext<Char>& formater, T& i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadIntAsBin(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename Char>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadIntAsHex(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadIntAsOct(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}






	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadUInt(BasicUnFormatContext<Char>& formater, T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = formater.BufferIsADigit();

		while (formater.BufferIsADigit())	{ res = res * 10 + (formater.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = res; }
		return isValid;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadUInt(BasicUnFormatContext<Char>& formater, T& i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsBin(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsHex(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T, typename Char>
	bool UnFormatContextReadUIntAsOct(BasicUnFormatContext<Char>& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}
}