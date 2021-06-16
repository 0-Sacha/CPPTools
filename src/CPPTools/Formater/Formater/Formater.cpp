
#include "CPPToolspch.h"

#include "Formater.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

namespace CPPTools::Fmt {

	Formater::Formater(const std::string_view format, char* const buffer, const std::size_t bufferSize)
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

	void Formater::CheckEndStr() {
		if (m_ColorMem.IsSetColor)
			FormatType<Detail::ResetAnsiColor>::Write(Detail::RESET_ANSI_COLOR, *this);
	}


	 std::uint8_t Formater::GetColorCode() {
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

	 std::uint8_t Formater::GetColorFG() {
		 std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		 std::uint8_t code = GetColorCode();
		if (code == std::numeric_limits<std::uint8_t>::max())
			code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else
			code += step;
		return code;
	}

	 std::uint8_t Formater::GetColorBG() {
		 std::uint8_t step = (std::uint8_t)(FormatIsEqualForward('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		 std::uint8_t code = GetColorCode();
		if (code == std::numeric_limits<std::uint8_t>::max())
			code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else
			code += step;
		return code;
	}

	void Formater::ColorValuePrint() {
		if (FormatIsEqualForward(':')) {
			FormatIgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			FormatParamGoTo(',');
			if (FormatIsEqualForward(',')) {
				FormatIgnoreSpace();
				color.Bg = (Detail::AnsiColorBG)GetColorBG();
			}
			FormatType<Detail::AnsiColor>::Write(color, *this);
		}
		else	FormatType<Detail::ResetAnsiColor>::Write(Detail::RESET_ANSI_COLOR, *this);
	}


	void Formater::TimerValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::high_resolution_clock::now() - GetAPI().GetTimeShift();
		FormatType<std::chrono::nanoseconds>::Write(ns, *this);
	}

	void Formater::DateValuePrint() {
		std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()) + GetAPI().GetHoursShift();
		FormatType<std::chrono::nanoseconds>::Write(ns, *this);
	}

	 std::uint8_t Formater::GetWordFromList(const char* formatTypes[],  std::uint8_t count) {
		 std::uint8_t res = std::numeric_limits< std::uint8_t>::max();
		for (int idx = 0; idx < count; ++idx) {
			if (FormatNextIsSame(formatTypes[idx])) {
				res = idx;
				idx = count;
			}
		}
		return res;
	}


	bool Formater::GetFormatType(const char* formatTypes[], FormatSpecifierIDX* arr,  std::uint8_t size)
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

	void Formater::ReloadColor()
	{
		if(m_ColorMem.IsSetColor) {
			if(m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor)
					FormatType<Detail::AnsiColor>::Write(m_ColorMem.Color, *this);
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColor24bBG>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::AnsiColorFG>::Write(m_ColorMem.Color.Fg, *this);
					FormatType<Detail::AnsiColorBG>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else if (m_ColorMem.FgType == Detail::AnsiColorFGType::AnsiColor24b) {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColor24bFG>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG>::Write(m_ColorMem.Color.Bg, *this);
				}
				else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b)
					FormatType<Detail::AnsiColor24b>::Write(m_ColorMem.Color24bits, *this);
				else {
					FormatType<Detail::AnsiColor24bFG>::Write(m_ColorMem.Color24bits.Fg, *this);
					FormatType<Detail::AnsiColorBG>::Write(Detail::AnsiColorBG::Default, *this);
				}

			} else {

				if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor) {
					FormatType<Detail::AnsiColorFG>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColorBG>::Write(m_ColorMem.Color.Bg, *this);
				} else if (m_ColorMem.BgType == Detail::AnsiColorBGType::AnsiColor24b) {
					FormatType<Detail::AnsiColorFG>::Write(Detail::AnsiColorFG::Default, *this);
					FormatType<Detail::AnsiColor24bBG>::Write(m_ColorMem.Color24bits.Bg, *this);
				} else {
					FormatType<Detail::ResetAnsiColor>::Write(Detail::RESET_ANSI_COLOR, *this);
				}

			}
		} else
			FormatType<Detail::ResetAnsiColor>::Write(Detail::RESET_ANSI_COLOR, *this);
	}
}