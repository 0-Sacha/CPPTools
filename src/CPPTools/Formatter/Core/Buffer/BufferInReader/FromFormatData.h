#pragma once

#include "../BufferIn.h"
#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt::Detail {

	//------------------ Buffer Read Int ------------------//
	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	bool FormatterMemoryBufferIn<CharBuffer>::ReadIntFormatData(T& i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			switch (formatData.IntPrint) {
			case ValueIntPrint::Int:
				if (formatData.ShiftType == ShiftType::Nothing) return FastReadInt(i);
				else											return BasicReadInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint);
			case ValueIntPrint::Bin:
				return BasicReadIntAsBin(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue);
			case ValueIntPrint::Hex:
				return BasicReadIntAsHex(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue, formatData.PrintStyle);
			case ValueIntPrint::Oct:
				return BasicReadIntAsOct(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue);
			}
		}
		return FastReadInt(i);
	}

	//------------------ Buffer Read UInt ------------------//
	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	bool FormatterMemoryBufferIn<CharBuffer>::ReadUIntFormatData(T& i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			switch (formatData.IntPrint) {
			case ValueIntPrint::Int:
				if (formatData.ShiftType == ShiftType::Nothing) return FastReadUInt(i);
				else											return BasicReadUInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint);
			case ValueIntPrint::Bin:
				return BasicReadIntAsBin(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue);
			case ValueIntPrint::Hex:
				return BasicReadIntAsHex(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue, formatData.PrintStyle);
			case ValueIntPrint::Oct:
				return BasicReadIntAsOct(i, formatData.DigitSize, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint, formatData.TrueValue);
			}
		}
		return FastReadUInt(i);
	}

	//------------------ Buffer Read Float ------------------//
	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	bool FormatterMemoryBufferIn<CharBuffer>::ReadFloatFormatData(T& i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			if (formatData.ShiftType == ShiftType::Nothing)	return FastReadFloat(i, formatData.FloatPrecision);
			else											return BasicReadFloat(i, formatData.FloatPrecision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint);
		}
		return FastReadFloat(i, formatData.FloatPrecision);
	}

}