#pragma once

#include "../BasicUnFormatContext.h"
#include "Integer.h"
#include "Float.h"

namespace CPPTools::Fmt {


	//------------------ C-Style String ------------------//
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharStr>
	inline bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferReadCharPt(CharStr* str, std::size_t size) {
		while (size-- != 0) *str++ = BufferGetAndForward();
		return true;
	}
	
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharStr>
	inline bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferReadCharPt(CharStr* str) {
		return false;
	}	


	//------------------ Buffer Read Int ------------------//
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferReadInt(T& i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadInt(*this, i); }
				else														{ return FormatFunc::UnFormatContextReadInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadIntAsBin(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadIntAsHex(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadIntAsOct(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			}
		}
		return FormatFunc::UnFormatContextReadInt(*this, i);
	}

	//------------------ Buffer Read UInt ------------------//
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferReadUInt(T& i) {
		if (m_FormatData.HasSpec) {
			switch (m_FormatData.IntPrint) {
			case Detail::ValueIntPrint::Int:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadUInt(*this, i); }
				else														{ return FormatFunc::UnFormatContextReadUInt(*this, i, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Bin:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadUIntAsBin(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadUIntAsBin(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Hex:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadUIntAsHex(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadUIntAsHex(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			case Detail::ValueIntPrint::Oct:
				if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadUIntAsOct(*this, i, m_FormatData.Precision); }
				else														{ return FormatFunc::UnFormatContextReadUIntAsOct(*this, i, m_FormatData.Precision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
			}
		}
		return FormatFunc::UnFormatContextReadUInt(*this, i);
	}

	//------------------ Buffer Read Float ------------------//
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename T>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferReadFloat(T& i) {
		if (m_FormatData.HasSpec) {
			if (m_FormatData.ShiftType == Detail::ShiftType::Nothing)	{ return FormatFunc::UnFormatContextReadFloat(*this, i, m_FormatData.FloatPrecision); }
			else														{ return FormatFunc::UnFormatContextReadFloat(*this, i, m_FormatData.FloatPrecision, m_FormatData.ShiftType, m_FormatData.ShiftValue, m_FormatData.ShiftPrint); }
		}
		return FormatFunc::UnFormatContextReadFloat(*this, i, m_FormatData.FloatPrecision);
	}

}