
#include "CPPToolspch.h"

#include "Formater.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

namespace CPPTools::Fmt {

	Formater::Formater(const char* const format, std::size_t formatSize, char* const buffer, std::size_t bufferSize)
		: m_Buffer(buffer), m_SubBuffer(buffer), m_BufferSize(bufferSize), m_Format(format), m_SubFormat(format), m_FormatSize(formatSize)
	{
	}

	void Formater::FormatPrintRec(std::uint8_t idx) { }
	void Formater::ParameterDataRec(std::uint8_t idx) { }
	void Formater::FormatPrintRecNamedArgs(const char* const name) { }


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
		 std::uint8_t step = (std::uint8_t)(FormatIsEqNext('+') ? Detail::AnsiColorFG::DBStep : Detail::AnsiColorFG::DStep);
		 std::uint8_t code = GetColorCode();
		if (code == std::numeric_limits<std::uint8_t>::max())
			code = (std::uint8_t)Detail::AnsiColorFG::Default;
		else
			code += step;
		return code;
	}

	 std::uint8_t Formater::GetColorBG() {
		 std::uint8_t step = (std::uint8_t)(FormatIsEqNext('+') ? Detail::AnsiColorBG::DBStep : Detail::AnsiColorBG::DStep);
		 std::uint8_t code = GetColorCode();
		if (code == std::numeric_limits<std::uint8_t>::max())
			code = (std::uint8_t)Detail::AnsiColorBG::Default;
		else
			code += step;
		return code;
	}

	void Formater::ColorValuePrint() {
		if (FormatIsEqNext(':')) {
			FormatParamIgnoreSpace();
			Detail::AnsiColor color;
			color.Fg = (Detail::AnsiColorFG)GetColorFG();
			FormatParamGoTo(',');
			if (FormatIsEqNext(',')) {
				FormatParamIgnoreSpace();
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


	bool Formater::FormatNextIsSame(const char* str) {
		const char* format = m_SubFormat;
		bool isSame = true;
		while (isSame && *str != 0 && *format != '}') {
			if (*str++ != *format++)
				isSame = false;
		}
		if (isSame && *str != 0)
			isSame = false;
		if (isSame)
			m_SubFormat = format;
		return isSame;
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
			FormatNext();
			FormatParamIgnoreSpace();

			for(std::uint8_t i = 0; i < size; ++i) {
				const char* fType = formatTypes[i];
				std::uint8_t idxType = 0;
				while (*fType != 0 && i != size) {
					if(FormatIsEqNext(*fType)) {
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