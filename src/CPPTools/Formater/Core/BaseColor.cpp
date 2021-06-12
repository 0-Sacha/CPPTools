
#include "CPPToolspch.h"
#include "BaseColor.h"

namespace CPPTools::Fmt {

	AnsiColor::AnsiColor(AnsiColorFG _fg, AnsiColorBG _bg)
		: Fg(_fg), Bg(_bg) { }

	Color24b::Color24b(uint8_t _r, uint8_t _g, uint8_t _b)
		: R(_r), G(_g), B(_b) { }

	AnsiColor24bFG::AnsiColor24bFG(uint8_t _r, uint8_t _g, uint8_t _b) : Color24b(_r, _g, _b) {};
	AnsiColor24bBG::AnsiColor24bBG(uint8_t _r, uint8_t _g, uint8_t _b) : Color24b(_r, _g, _b) {};

	AnsiColor24b::AnsiColor24b(AnsiColor24bFG _fg, AnsiColor24bBG _bg)
		: Fg(_fg), Bg(_bg) { }


	AnsiColorMem::AnsiColorMem()
		: IsSetColor(false), FgType(AnsiColorFGType::Nothing), BgType(AnsiColorBGType::Nothing) {}

}