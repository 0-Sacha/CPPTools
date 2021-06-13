#pragma once

#include "../Formater.h"

#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt {

	template<typename T>
	void Formater::BufferParseInt(T i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseInt(*this, i); return; }
				else														{ FormatFunc::FormaterParseInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseIntAsBin(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormaterParseIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseIntAsHex(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormaterParseIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseIntAsOct(*this, i, m_FormatData.Precision); return; }
				else														{ FormatFunc::FormaterParseIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			}
		}
		FormatFunc::FormaterParseInt(*this, i);
	}

	template<typename T>
	void Formater::BufferParseUInt(T i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
				case Detail::ValueIntPrint::Int:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseUInt(*this, i); return; }
					else														{ FormatFunc::FormaterParseUInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Bin:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseUIntAsBin(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormaterParseUIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Hex:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseUIntAsHex(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormaterParseUIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
				case Detail::ValueIntPrint::Oct:
					if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseUIntAsOct(*this, i, m_FormatData.Precision); return; }
					else														{ FormatFunc::FormaterParseUIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
			}
		}
		FormatFunc::FormaterParseUInt(*this, i);
	}

	template<typename T>
	void Formater::BufferParseFloat(T i) {
		if (m_FormatData.HasSpec) {
			if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ FormatFunc::FormaterParseFloat(*this, i, m_FormatData.FloatPrecision); return; }
			else														{ FormatFunc::FormaterParseFloat(*this, i, m_FormatData.FloatPrecision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); return; }
		}
		FormatFunc::FormaterParseFloat(*this, i, m_FormatData.FloatPrecision);
	}
}