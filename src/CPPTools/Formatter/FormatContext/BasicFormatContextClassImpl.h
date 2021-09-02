#pragma once

#include "BasicFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/BaseSTDLib.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicFormatContext(const std::basic_string_view<CharFormat> format, CharBuffer* const buffer, const std::size_t bufferSize, ContextArgs&& ...args)
		: m_BufferOut(buffer, bufferSize)
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicFormatContext(const bool bufferIsAutoResize, const std::basic_string_view<CharFormat> format, ContextArgs &&...args)
		: m_BufferOut()
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ParentCharFormat, typename ...ParentContextArgs>
	BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicFormatContext(const std::basic_string_view<CharFormat> format, BasicFormatContext<ParentCharFormat, CharBuffer, ParentContextArgs...>& parentContext, ContextArgs&& ...args)
		: m_BufferOut(parentContext.BufferOut())
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_NoStride(parentContext.GetNoStride())
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(parentContext.GetFormatData());
		m_ColorMem = parentContext.GetColorMem();
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ChildCharFormat, typename ...ChildContextArgs>
	inline void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::UpdateContextFromChild(BasicFormatContext<ChildCharFormat, CharBuffer, ChildContextArgs...>& childContext) {
		m_BufferOut.UpdateFromClidBuffer(childContext.BufferOut());
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
		std::uint8_t step = (std::uint8_t)(FormatStr().IsEqualForward('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)()) code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else													code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	std::uint8_t BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetColorBG() {
		std::uint8_t step = (std::uint8_t)(FormatStr().IsEqualForward('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())	code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else													code += step;
		return code;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>::ColorValuePrint() {
		if (m_FormatStr.IsEqualForward(':')) {
			m_FormatStr.IgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			m_FormatStr.ParamGoTo(',');
			if (m_FormatStr.IsEqualForward(',')) {
				m_FormatStr.IgnoreSpace();
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
			if (m_FormatStr.NextIsSame(formatTypes[idx])) {
				res = idx; idx = SIZE;
			}
		}
		return res;	
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