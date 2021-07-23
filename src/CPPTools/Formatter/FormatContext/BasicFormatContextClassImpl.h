#pragma once

#include "BasicFormatContextHelperFile.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/BaseSTDLib.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicFormatContext(const std::basic_string_view<CharFormat> format, CharBuffer* const buffer, const std::size_t bufferSize, ContextArgs&& ...args)
		: m_Buffer(buffer)
		, m_SubBuffer(buffer)
		, m_BufferEnd(buffer + bufferSize)
		, m_BufferSize(bufferSize)
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ContextArgsSize(sizeof...(ContextArgs))
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
		*(m_BufferEnd - 1) = 0;
	}


	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename OldCharFormat, typename ...OldContextArgs>
	BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicFormatContext(const std::basic_string_view<CharFormat> format, BasicFormatContext<OldCharFormat, CharBuffer, OldContextArgs...>& oldContext, ContextArgs&& ...args)
		: m_Buffer(oldContext.GetBuffer())
		, m_SubBuffer(oldContext.GetSubBuffer())
		, m_BufferEnd(oldContext.GetBufferEnd())
		, m_BufferSize(oldContext.GetBufferSize())
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ContextArgsSize(sizeof...(ContextArgs))
		, m_NoStride(oldContext.GetNoStride())
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(oldContext.GetFormatData());
		m_ColorMem = oldContext.GetColorMem();
	}


	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::CheckEndStr() {
		if (m_ColorMem.IsSetColor)
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetColorCode() {
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

		return (std::uint8_t)GetWordFromList(colorCode);
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetColorFG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)()) code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else													code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetColorBG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())	code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else													code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ColorValuePrint() {
		if (FormatIsEqualForward(':')) {
			FormatIgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			FormatParamGoTo(',');
			if (FormatIsEqualForward(',')) {
				FormatIgnoreSpace();
				color.Bg = (Detail::AnsiColorBG)GetColorBG();
			}
			FormatType<Detail::AnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(color, *this);
		}
		else	FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::TimerValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::high_resolution_clock::now() - GetAPI().GetTimeShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(ns, *this);
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::DateValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()) + GetAPI().GetHoursShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(ns, *this);
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharList, std::size_t SIZE>
	std::size_t BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetWordFromList(const std::basic_string_view<CharList> (&formatTypes)[SIZE]) {
		std::uint8_t res = (std::numeric_limits<std::uint8_t>::max)();
		for (int idx = 0; idx < SIZE; ++idx) {
			if (FormatNextIsSame(formatTypes[idx])) {
				res = idx; idx = SIZE;
			}
		}
		return res;	
	}


	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharToTest>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatNextIsANamedArgs(std::basic_string_view<CharToTest> sv) {
		const CharFormat* const prevSubFormat = m_SubFormat;
		if (FormatNextIsSame(sv) && (FormatIsEqualTo(':') || FormatIsEqualTo('}')))
			return true;
		m_SubFormat = prevSubFormat;
		return false;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharToTest>
	bool BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatNextIsSame(std::basic_string_view<CharToTest> sv) {
		const CharToTest* str = sv.data();
		std::size_t size = sv.size();
		const CharFormat* prevSubFormat = m_SubFormat;				bool isSame = true;
		while (isSame && size-- != 0 && FormatCanMoveForward())		isSame = FormatGetAndForwardNoCheck() == *str++;
		if (isSame && size == 0)									isSame = false;
		if (!isSame)												m_SubFormat = prevSubFormat;
		return isSame;
	}
	
	/////---------- ReloadColor ----------/////
	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReloadColor()
	{
		if(m_ColorMem.IsSetColor) {
			if(m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor)
					FormatType<Detail::AnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color, *this);
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else if (m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor24b) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color.Bg, *this);
				}
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b)
					FormatType<Detail::AnsiColor24b, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color24bits, *this);
				else {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color.Bg, *this);
				} else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::RESET_ANSI_COLOR, *this);
				}

			}
		} else
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}
}