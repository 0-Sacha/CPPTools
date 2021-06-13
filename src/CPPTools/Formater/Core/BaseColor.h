#pragma once

#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt::Detail {

	enum class AnsiColorFG : std::uint8_t {
		Black = 30,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		BrightBlack = 90,
		BrightRed,
		BrightGreen,
		BrightYellow,
		BrightBlue,
		BrightMagenta,
		BrightCyan,
		BrightWhite,
		Default = White,
		DStep = Black,
		DBStep = BrightBlack
	};

	enum class AnsiColorBG : std::uint8_t {
		Black = 40,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		BrightBlack = 100,
		BrightRed,
		BrightGreen,
		BrightYellow,
		BrightBlue,
		BrightMagenta,
		BrightCyan,
		BrightWhite,
		Default = Black,
		DStep = Black,
		DBStep = BrightBlack
	};

	struct ResetAnsiColor {};

	struct AnsiColor {
		AnsiColor(AnsiColorFG _fg = AnsiColorFG::Default, AnsiColorBG _bg = AnsiColorBG::Default);
		AnsiColorFG Fg;
		AnsiColorBG Bg;
	};

	struct Color24b {
		Color24b(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b);
		std::uint8_t R, G, B;
	};

	struct AnsiColor24bFG : public Color24b {
		AnsiColor24bFG(std::uint8_t _r = 255, std::uint8_t _g = 255, std::uint8_t _b = 255);
	};

	struct AnsiColor24bBG : public Color24b {
		AnsiColor24bBG(std::uint8_t _r = 0, std::uint8_t _g = 0, std::uint8_t _b = 0);
	};


	struct AnsiColor24b {
		AnsiColor24b(AnsiColor24bFG _fg = AnsiColor24bFG(), AnsiColor24bBG _bg = AnsiColor24bBG());
		AnsiColor24bFG Fg;
		AnsiColor24bBG Bg;
	};

	enum class AnsiColorFGType : std::uint8_t {
		Nothing,
		AnsiColor,
		AnsiColor24b
	};

	enum class AnsiColorBGType : std::uint8_t {
		Nothing,
		AnsiColor,
		AnsiColor24b
	};

	struct AnsiColorMem
	{
		AnsiColorMem();
		bool IsSetColor;
		AnsiColorFGType FgType;
		AnsiColorBGType BgType;
		AnsiColor24b Color24bits;
		AnsiColor Color;
	};

	inline ResetAnsiColor RESET_ANSI_COLOR;
}