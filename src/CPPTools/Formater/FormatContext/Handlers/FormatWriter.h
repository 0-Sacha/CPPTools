#pragma once

#include "../BasicFormatContext.h"

#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt {
	
	template<typename CharFormat, typename CharBuffer>
	template<typename T>
	void BasicFormatContext<CharFormat, CharBuffer>::BufferWriteInt(T i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteInt(*this, i); return; }
				else														{ FormatFunc::FormatContextWriteInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteIntAsBin(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormatContextWriteIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteIntAsHex(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormatContextWriteIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteIntAsOct(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormatContextWriteIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			}
		}
		FormatFunc::FormatContextWriteInt(*this, i);
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename T>
	void BasicFormatContext<CharFormat, CharBuffer>::BufferWriteUInt(T i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
				case Detail::ValueIntPrint::Int:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteUInt(*this, i); return; }
					else														{ FormatFunc::FormatContextWriteUInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Bin:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteUIntAsBin(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormatContextWriteUIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Hex:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteUIntAsHex(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormatContextWriteUIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Oct:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteUIntAsOct(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormatContextWriteUIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			}
		}
		FormatFunc::FormatContextWriteUInt(*this, i);
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename T>
	void BasicFormatContext<CharFormat, CharBuffer>::BufferWriteFloat(T i) {
		if (m_FormatData.HasSpec) {
			if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormatContextWriteFloat(*this, i, m_FormatData.FloatPrecision); return; }
			else														{ FormatFunc::FormatContextWriteFloat(*this, i, m_FormatData.FloatPrecision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
		}
		FormatFunc::FormatContextWriteFloat(*this, i, m_FormatData.FloatPrecision);
	}
}