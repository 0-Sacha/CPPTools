#pragma once


#include "../UnFormater.h"

namespace CPPTools::Fmt::FormatFunc {


	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T>
	bool FormaterUnParseInt(UnFormater& formater, T& i) {
		T res = 0;
		bool sign = false, isValid = false;

		sign = formater.BufferIsEqualForward('-');
		isValid = formater.BufferIsADigit();

		while (formater.BufferIsADigit())	{ res = res * 10 + (formater.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = sign ? -res : res; }
		return isValid;
	}

	template<typename T>
	bool FormaterUnParseInt(UnFormater& formater, T& i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T>
	bool FormaterUnParseIntAsBin(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseIntAsBin(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T>
	bool FormaterUnParseIntAsHex(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseIntAsHex(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T>
	bool FormaterUnParseIntAsOct(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseIntAsOct(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}






	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T>
	bool FormaterUnParseUInt(UnFormater& formater, T& i) {
		T res = (T)0;
		bool isValid = false;

		isValid = formater.BufferIsADigit();

		while (formater.BufferIsADigit())	{ res = res * 10 + (formater.BufferGetAndForward() - '0'); }

		if (isValid)						{ i = res; }
		return isValid;
	}

	template<typename T>
	bool FormaterUnParseUInt(UnFormater& formater, T& i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Bin --------------------//

	template<typename T>
	bool FormaterUnParseUIntAsBin(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseUIntAsBin(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Hex --------------------//


	template<typename T>
	bool FormaterUnParseUIntAsHex(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseUIntAsHex(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}


	//-------------------- UInt Oct --------------------//

	template<typename T>
	bool FormaterUnParseUIntAsOct(UnFormater& formater, T& i, std::int16_t def) {
		return false;
	}

	template<typename T>
	bool FormaterUnParseUIntAsOct(UnFormater& formater, T& i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		return false;
	}
}