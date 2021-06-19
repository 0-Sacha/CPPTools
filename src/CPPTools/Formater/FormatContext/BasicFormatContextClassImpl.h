#pragma once

#include "BasicFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/String.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer>
	BasicFormatContext<CharFormat, CharBuffer>::BasicFormatContext(const std::basic_string_view<CharFormat> format, CharBuffer* const buffer, const std::size_t bufferSize)
		: m_Buffer(buffer)
		, m_SubBuffer(buffer)
		, m_BufferEnd(buffer + bufferSize)
		, m_BufferSize(bufferSize)
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
		*(m_BufferEnd - 1) = 0;
	}


	template<typename CharFormat, typename CharBuffer>
	template<typename OldCharFormat>
	BasicFormatContext<CharFormat, CharBuffer>::BasicFormatContext(const std::basic_string_view<CharFormat> format, const BasicFormatContext<OldCharFormat, CharBuffer>& oldContext)
		: m_Buffer(oldContext.m_Buffer)
		, m_SubBuffer(oldContext.m_SubBuffer)
		, m_BufferEnd(oldContext.m_BufferEnd)
		, m_BufferSize(oldContext.m_BufferSize)
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_NoStride(oldContext.m_NoStride)
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(oldContext.m_FormatData);
		m_ColorMem = oldContext.m_ColorMem;
	}


	template<typename CharFormat, typename CharBuffer>
	void BasicFormatContext<CharFormat, CharBuffer>::CheckEndStr() {
		if (m_ColorMem.IsSetColor)
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename CharFormat, typename CharBuffer>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer>::GetColorCode() {
		static constexpr std::string_view colorCode[8] = {
			"black",
			"red",
			"green",
			"yellow",
			"blue",
			"magenta",
			"cyan",
			"white",
		};

		return GetWordFromList(colorCode);
	}

	template<typename CharFormat, typename CharBuffer>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer>::GetColorFG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())
			code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else												
			code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer>::GetColorBG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())	code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else													code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer>
	void BasicFormatContext<CharFormat, CharBuffer>::ColorValuePrint() {
		if (FormatIsEqualForward(':')) {
			FormatIgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			FormatParamGoTo(',');
			if (FormatIsEqualForward(',')) {
				FormatIgnoreSpace();
				color.Bg = (Detail::AnsiColorBG)GetColorBG();
			}
			FormatType<Detail::AnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(color, *this);
		}
		else	FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename CharFormat, typename CharBuffer>
	void BasicFormatContext<CharFormat, CharBuffer>::TimerValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::high_resolution_clock::now() - GetAPI().GetTimeShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<CharFormat, CharBuffer>>::Write(ns, *this);
	}

	template<typename CharFormat, typename CharBuffer>
	void BasicFormatContext<CharFormat, CharBuffer>::DateValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()) + GetAPI().GetHoursShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<CharFormat, CharBuffer>>::Write(ns, *this);
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename CharList, std::size_t SIZE>
	std::size_t BasicFormatContext<CharFormat, CharBuffer>::GetWordFromList(const std::basic_string_view<CharList> (&formatTypes)[SIZE]) {
		std::uint8_t res = (std::numeric_limits<std::uint8_t>::max)();
		for (int idx = 0; idx < SIZE; ++idx) {
			if (FormatNextIsSame(formatTypes[idx])) {
				res = idx; idx = SIZE;
			}
		}
		return res;	
	}


	template<typename CharFormat, typename CharBuffer>
	template<typename CharToTest>
	bool BasicFormatContext<CharFormat, CharBuffer>::FormatNextIsANamedArgs(std::basic_string_view<CharToTest> sv) {
		const CharFormat* const prevSubFormat = m_SubFormat;
		if (FormatNextIsSame(sv) && FormatIsEqualTo(':') || FormatIsEqualTo('}'))
			return true;
		m_SubFormat = prevSubFormat;
		return false;
	}

	template<typename CharFormat, typename CharBuffer>
	template<typename CharToTest>
	bool BasicFormatContext<CharFormat, CharBuffer>::FormatNextIsSame(std::basic_string_view<CharToTest> sv) {
		const CharToTest* str = sv.data();
		std::size_t size = sv.size();
		const CharFormat* prevSubFormat = m_SubFormat;			bool isSame = true;
		while (isSame && size-- != 0 && FormatCanMoveForward())
			isSame = FormatGetAndForwardNoCheck() == *str++;
		if (isSame && size == 0)
			isSame = false;
		if (!isSame)
			m_SubFormat = prevSubFormat;
		return isSame;
	}
	
	// FFIND - EXPERIMENTAL
	/////---------- FormatReadParameter ----------/////
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx) {}
	template<typename ValueType, typename T, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const T& t, Args&& ...args) {
		if (idx != 0)	GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::int8_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::uint8_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::int16_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::uint16_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::int32_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::uint32_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::int64_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	template<typename ValueType, typename ...Args>
	static inline void GetFormatValueAt(ValueType& value, FormatIdx idx, const std::uint64_t& t, Args&& ...args) {
		if (idx == 0)	value = t;
		else			GetFormatValueAt(value, idx - 1, std::forward<Args>(args)...);
	}
	
	template<typename CharFormat, typename CharBuffer>
	template<typename T, typename ...Args>
	bool BasicFormatContext<CharFormat, CharBuffer>::FormatReadParameter(T& i, Args&& ...args) {
		const CharFormat* const mainSubFormat = m_SubFormat;
		FormatIdx formatIdx;
		if (GetFormatIdx(formatIdx, std::forward<Args>(args)...)) {
			FormatForward();
			GetFormatValueAt(i, formatIdx, std::forward<Args>(args)...);
			return true;
		}
		m_SubFormat = mainSubFormat;
		return false;
	}


	/////---------- ReloadColor ----------/////
	template<typename CharFormat, typename CharBuffer>
	void BasicFormatContext<CharFormat, CharBuffer>::ReloadColor()
	{
		if(m_ColorMem.IsSetColor) {
			if(m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor)
					FormatType<Detail::AnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color, *this);
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else if (m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor24b) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color.Bg, *this);
				}
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b)
					FormatType<Detail::AnsiColor24b, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color24bits, *this);
				else {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color.Bg, *this);
				} else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<CharFormat, CharBuffer>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::RESET_ANSI_COLOR, *this);
				}

			}
		} else
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}
}