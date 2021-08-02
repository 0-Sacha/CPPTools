#pragma once

#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	void FormatterMemoryBufferOut<CharBuffer>::WriteIntFormatData(T i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			switch (formatData.IntPrint) {
			case ValueIntPrint::Int:
				if (formatData.ShiftType == ShiftType::Nothing) { BasicWriteInt(i); return; }
				else											{ BasicWriteInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Bin:
				if (formatData.ShiftType == ShiftType::Nothing) { BasicWriteIntAsBin(i, formatData.Precision); return; }
				else											{ BasicWriteIntAsBin(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Hex:
				if (formatData.ShiftType == ShiftType::Nothing) { BasicWriteIntAsHex(i, formatData.Precision); return; }
				else											{ BasicWriteIntAsHex(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Oct:
				if (formatData.ShiftType == ShiftType::Nothing) { BasicWriteIntAsOct(i, formatData.Precision); return; }
				else											{ BasicWriteIntAsOct(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			}
		}
		BasicWriteInt(i);
	}

	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	void FormatterMemoryBufferOut<CharBuffer>::WriteUIntFormatData(T i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			switch (formatData.IntPrint) {
			case ValueIntPrint::Int:
				if (formatData.ShiftType == ShiftType::Nothing)	{ BasicWriteUInt(i); return; }
				else											{ BasicWriteUInt(i, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Bin:
				if (formatData.ShiftType == ShiftType::Nothing)	{ BasicWriteUIntAsBin(i, formatData.Precision); return; }
				else											{ BasicWriteUIntAsBin(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Hex:
				if (formatData.ShiftType == ShiftType::Nothing)	{ BasicWriteUIntAsHex(i, formatData.Precision); return; }
				else											{ BasicWriteUIntAsHex(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			case ValueIntPrint::Oct:
				if (formatData.ShiftType == ShiftType::Nothing)	{ BasicWriteUIntAsOct(i, formatData.Precision); return; }
				else											{ BasicWriteUIntAsOct(i, formatData.Precision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
			}
		}
		BasicWriteUInt(i);
	}

	template<typename CharBuffer>
	template<typename T, typename FormatDataCharType>
	void FormatterMemoryBufferOut<CharBuffer>::WriteFloatFormatData(T i, const FormatData<FormatDataCharType>& formatData) {
		if (formatData.HasSpec) {
			if (formatData.ShiftType == ShiftType::Nothing)	{ BasicWriteFloat(i, formatData.FloatPrecision); return; }
			else											{ BasicWriteFloat(i, formatData.FloatPrecision, formatData.ShiftType, formatData.ShiftValue, formatData.ShiftPrint); return; }
		}
		BasicWriteFloat(i, formatData.FloatPrecision);
	}
}