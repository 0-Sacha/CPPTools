
#include "BasicFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/String.h"

namespace CPPTools::Fmt {

	template<typename Char>
	BasicFormatContext<Char>::BasicFormatContext(const std::basic_string_view<Char> format, Char* const buffer, const std::size_t bufferSize)
		: m_Buffer(buffer)
		, m_SubBuffer(buffer)
		, m_BufferEnd(buffer + bufferSize)
		, m_BufferSize(bufferSize)
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
	{
		*(m_BufferEnd - 1) = 0;
	}

	template<typename Char>
	void BasicFormatContext<Char>::CheckEndStr() {
		if (m_ColorMem.IsSetColor)
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<Char>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename Char>
	std::uint8_t BasicFormatContext<Char>::GetColorCode() {
		static const char* colorCode[8] = {
			"black",
			"red",
			"green",
			"yellow",
			"blue",
			"magenta",
			"cyan",
			"white",
		};

		return GetWordFromList(colorCode, 8);
	}

	template<typename Char>
	std::uint8_t BasicFormatContext<Char>::GetColorFG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())
			code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else												
			code += step;
		return code;
	}

	template<typename Char>
	std::uint8_t BasicFormatContext<Char>::GetColorBG() {
		std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		std::uint8_t code = GetColorCode();
		if (code == (std::numeric_limits<std::uint8_t>::max)())	code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else													code += step;
		return code;
	}

	template<typename Char>
	void BasicFormatContext<Char>::ColorValuePrint() {
		if (FormatIsEqualForward(':')) {
			FormatIgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			FormatParamGoTo(',');
			if (FormatIsEqualForward(',')) {
				FormatIgnoreSpace();
				color.Bg = (Detail::AnsiColorBG)GetColorBG();
			}
			FormatType<Detail::AnsiColor, BasicFormatContext<Char>>::Write(color, *this);
		}
		else	FormatType<Detail::ResetAnsiColor, BasicFormatContext<Char>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}

	template<typename Char>
	void BasicFormatContext<Char>::TimerValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::high_resolution_clock::now() - GetAPI().GetTimeShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<Char>>::Write(ns, *this);
	}

	template<typename Char>
	void BasicFormatContext<Char>::DateValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()) + GetAPI().GetHoursShift();
		FormatType<std::chrono::nanoseconds, BasicFormatContext<Char>>::Write(ns, *this);
	}

	template<typename Char>
	std::uint8_t BasicFormatContext<Char>::GetWordFromList(const char* formatTypes[], std::uint8_t count) {
		std::uint8_t res = (std::numeric_limits< std::uint8_t>::max)();
		for (int idx = 0; idx < count; ++idx) {
			if (FormatNextIsSame(formatTypes[idx])) {
				res = idx;
				idx = count;
			}
		}
		return res;
	}


	template<typename Char>
	bool BasicFormatContext<Char>::GetFormatType(const char* formatTypes[], FormatSpecifierIDX* arr,  std::uint8_t size)
	{
		if (FormatIsEndOfParameter())		return false;
		while (!FormatIsEndOfParameter()) {
			FormatForward();
			FormatIgnoreSpace();

			for(std::uint8_t i = 0; i < size; ++i) {
				const char* fType = formatTypes[i];
				std::uint8_t idxType = 0;
				while (*fType != 0 && i != size) {
					if(FormatIsEqualForward(*fType)) {
						arr[i].Type = *fType;
						FormatReadInt(arr[i].Value);
						arr[i].IdxType = idxType;
						i = size;
					}
					++fType;
					++idxType;
				}
			}

			FormatParamGoTo(',');
		}
		return true;
	}

	template<typename Char>
	void BasicFormatContext<Char>::ReloadColor()
	{
		if(m_ColorMem.IsSetColor) {
			if(m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor)
					FormatType<Detail::AnsiColor, BasicFormatContext<Char>>::Write(m_ColorMem.Color, *this);
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<Char>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<Char>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<Char>>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<Char>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else if (m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor24b) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<Char>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<Char>>::Write(m_ColorMem.Color.Bg, *this);
				}
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b)
					FormatType<Detail::AnsiColor24b, BasicFormatContext<Char>>::Write(m_ColorMem.Color24bits, *this);
				else {
					FormatType<Detail::AnsiColor24bFG, BasicFormatContext<Char>>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<Char>>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<Char>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColorBG, BasicFormatContext<Char>>::Write(m_ColorMem.Color.Bg, *this);
				} else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG, BasicFormatContext<Char>>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColor24bBG, BasicFormatContext<Char>>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::ResetAnsiColor, BasicFormatContext<Char>>::Write(Detail::RESET_ANSI_COLOR, *this);
				}

			}
		} else
			FormatType<Detail::ResetAnsiColor, BasicFormatContext<Char>>::Write(Detail::RESET_ANSI_COLOR, *this);
	}
}