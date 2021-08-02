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
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadInt(i); }
				else											{ return BasicReadInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Bin:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadIntAsBin(i, formatData.Precision); }
				else											{ return BasicReadIntAsBin(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Hex:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadIntAsHex(i, formatData.Precision); }
				else											{ return BasicReadIntAsHex(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Oct:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadIntAsOct(i, formatData.Precision); }
				else											{ return BasicReadIntAsOct(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			}
		}
		return BasicReadInt(i);
	}

	//------------------ Buffer Read UInt ------------------//
	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	bool FormatterMemoryBufferIn<CharBuffer>::ReadUIntFormatData(T& i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			switch (formatData.IntPrint) {
			case ValueIntPrint::Int:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadUInt(i); }
				else											{ return BasicReadUInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Bin:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadUIntAsBin(i, formatData.Precision); }
				else											{ return BasicReadUIntAsBin(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Hex:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadUIntAsHex(i, formatData.Precision); }
				else											{ return BasicReadUIntAsHex(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			case ValueIntPrint::Oct:
				if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadUIntAsOct(i, formatData.Precision); }
				else											{ return BasicReadUIntAsOct(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
			}
		}
		return BasicReadUInt(i);
	}

	//------------------ Buffer Read Float ------------------//
	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	bool FormatterMemoryBufferIn<CharBuffer>::ReadFloatFormatData(T& i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			if (formatData.ShiftType == ShiftType::Nothing)	{ return BasicReadFloat(i, formatData.FloatPrecision); }
			else											{ return BasicReadFloat(i, formatData.FloatPrecision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); }
		}
		return BasicReadFloat(i, formatData.FloatPrecision);
	}

}