#pragma once

#include "../UnFormater.h"
#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt {

	template<typename T>
	bool UnFormater::BufferUnParseInt(T& i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseInt(*this, i); }
				else														{ return FormatFunc::FormaterUnParseInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseIntAsBin(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseIntAsHex(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseIntAsOct(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			}
		}
		return FormatFunc::FormaterUnParseInt(*this, i);
	}

	template<typename T>
	bool UnFormater::BufferUnParseUInt(T& i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseUInt(*this, i); }
				else														{ return FormatFunc::FormaterUnParseUInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseUIntAsBin(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseUIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseUIntAsHex(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseUIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseUIntAsOct(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::FormaterUnParseUIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			}
		}
		return FormatFunc::FormaterUnParseUInt(*this, i);
	}

	template<typename T>
	bool UnFormater::BufferUnParseFloat(T& i) {
		if (m_FormatData.HasSpec) {
			if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::FormaterUnParseFloat(*this, i, m_FormatData.FloatPrecision); }
			else														{ return FormatFunc::FormaterUnParseFloat(*this, i, m_FormatData.FloatPrecision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
		}
		return FormatFunc::FormaterUnParseFloat(*this, i, m_FormatData.FloatPrecision);
	}

}