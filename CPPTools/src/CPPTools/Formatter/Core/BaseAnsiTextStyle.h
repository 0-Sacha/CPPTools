#pragma once

#include "CPPTools/Core/Core.h"
#include "BaseAnsiTextColor.h"

namespace CPPTools::Fmt::Detail {

	// According to : https://en.wikipedia.org/wiki/ANSI_escape_code

	struct ResetAnsiAllParameters {};

	struct ResetAnsiStyle {};


	enum class AnsiTFSIntensity : std::uint8_t {
		Bold = 1,
		Dim = 2,
		Normal = 22
	};

	enum class AnsiTFSItalic : std::uint8_t {
		Enable = 3,
		Disable = 23
	};

	enum class AnsiTFSUnderline : std::uint8_t {
		Underlined		= 4,
		DoubleUnerlined	= 21, // may only disable bold
		Disable			= 24
	};
	
	enum class AnsiTFSBlink : std::uint8_t {
		SlowBlink	= 5,
		FastBlink	= 6,
		Disable		= 25
	};

	enum class AnsiTFSInverted : std::uint8_t {
		Enable	= 7,
		Disable = 27
	};

	enum class AnsiTFSIdeogram : std::uint8_t {
		Underlined = 60,
		DoubleUnderlined = 61,
		Overlined = 62,
		DoubleOverlined = 63,
		StressMarking = 64,
		AllDisable = 65
	};

	enum class AnsiTFSScript : std::uint8_t {
		Superscript	= 74,
		Subscript	= 75,
		AllDisable	= 76
	};

	struct ForwardAsAnsiBasicTextStyle {
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSIntensity value)	: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSItalic value)		: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSUnderline value)	: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSBlink value)		: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSInverted value)		: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSIdeogram value)		: Value(static_cast<std::uint8_t>(value)) {}
		inline ForwardAsAnsiBasicTextStyle(const AnsiTFSScript value)		: Value(static_cast<std::uint8_t>(value)) {}

		std::uint8_t Value;

		inline operator std::uint8_t() const {
			return Value;
		}
	};

	enum class AnsiBasicTextStyle : std::uint8_t {
		Intensity_Bold				= static_cast<std::uint8_t>(AnsiTFSIntensity::Bold),
		Intensity_Dim				= static_cast<std::uint8_t>(AnsiTFSIntensity::Dim),
		Intensity_Normal			= static_cast<std::uint8_t>(AnsiTFSIntensity::Normal),

		Italic_Enable				= static_cast<std::uint8_t>(AnsiTFSItalic::Enable),
		Italic_Disable				= static_cast<std::uint8_t>(AnsiTFSItalic::Disable),

		Underline_Underlined		= static_cast<std::uint8_t>(AnsiTFSUnderline::Underlined),
		Underline_DoubleUnerlined	= static_cast<std::uint8_t>(AnsiTFSUnderline::DoubleUnerlined), // may only disable bold
		Underline_Disable			= static_cast<std::uint8_t>(AnsiTFSUnderline::Disable),
		Underline_SelectColor		= 255,

		Blink_SlowBlink				= static_cast<std::uint8_t>(AnsiTFSBlink::SlowBlink),
		Blink_FastBlink				= static_cast<std::uint8_t>(AnsiTFSBlink::FastBlink),
		Blink_Disable				= static_cast<std::uint8_t>(AnsiTFSBlink::Disable),

		Inverted_Enable				= static_cast<std::uint8_t>(AnsiTFSInverted::Enable),
		Inverted_Disable			= static_cast<std::uint8_t>(AnsiTFSInverted::Disable),

		Ideogram_Underlined			= static_cast<std::uint8_t>(AnsiTFSIdeogram::Underlined),
		Ideogram_DoubleUnderlined	= static_cast<std::uint8_t>(AnsiTFSIdeogram::DoubleUnderlined),
		Ideogram_Overlined			= static_cast<std::uint8_t>(AnsiTFSIdeogram::Overlined),
		Ideogram_DoubleOverlined	= static_cast<std::uint8_t>(AnsiTFSIdeogram::DoubleOverlined),
		Ideogram_StressMarking		= static_cast<std::uint8_t>(AnsiTFSIdeogram::StressMarking),
		Ideogram_AllDisable			= static_cast<std::uint8_t>(AnsiTFSIdeogram::AllDisable),

		Script_Superscript			= static_cast<std::uint8_t>(AnsiTFSScript::Superscript),
		Script_Subscript			= static_cast<std::uint8_t>(AnsiTFSScript::Subscript),
		Script_AllDisable			= static_cast<std::uint8_t>(AnsiTFSScript::AllDisable)
	};

	struct ResetAnsiUnderlineColor {};

	struct AnsiUnderlineColor24b : public AnsiColor24bType {
		AnsiUnderlineColor24b(std::uint8_t r = 0, std::uint8_t g = 0, std::uint8_t b = 0)
			: AnsiColor24bType(r, g, b) {};
	};

	struct AnsiNColorUnderline : public AnsiNColorType {
		AnsiNColorUnderline(const std::uint8_t color = 0)	: AnsiNColorType(color) {}
		AnsiNColorUnderline(const AnsiNColorType color)		: AnsiNColorType(color.GetColor()) {}

	public:
		static AnsiNColorUnderline MakeNormalColor(const std::uint8_t value) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::MakeNormalColor(value));
		}

		static AnsiNColorUnderline MakeBrightColor(const std::uint8_t value) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::MakeBrightColor(value));
		}

		static AnsiNColorUnderline Make666CubeColor255(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::Make666CubeColor255(r, g, b));
		}

		static AnsiNColorUnderline Make666CubeColor5(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::Make666CubeColor5(r, g, b));
		}

		static AnsiTextNColorFg MakeGrayscaleColor255(const std::uint8_t value) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::MakeGrayscaleColor255(value));
		}

		static AnsiNColorUnderline MakeGrayscaleColor24(const std::uint8_t value) {
			return static_cast<AnsiNColorUnderline>(AnsiNColorType::MakeGrayscaleColor24(value));
		}
	};

	enum class AnsiColorUnderlineType : std::uint8_t {
		Default,
		AnsiTextNColor,
		AnsiTextColor24b
	};

	struct AnsiTextCurrentStyle {
	public:
		AnsiTextCurrentStyle() {}

		AnsiTFSIntensity		Intensity			= AnsiTFSIntensity::Normal;
		AnsiTFSItalic			Italic				= AnsiTFSItalic::Disable;
		AnsiTFSUnderline		Underline			= AnsiTFSUnderline::Disable;
		AnsiColorUnderlineType	UnderlineColorType	= AnsiColorUnderlineType::Default;
		AnsiNColorUnderline		UnderlineColorN;
		AnsiUnderlineColor24b	UnderlineColor24bits;
		AnsiTFSBlink			Blink				= AnsiTFSBlink::Disable;
		AnsiTFSInverted			Inverted			= AnsiTFSInverted::Disable;
		AnsiTFSIdeogram			Ideogram			= AnsiTFSIdeogram::AllDisable;
		AnsiTFSScript			Script				= AnsiTFSScript::AllDisable;
	};

	struct AnsiTextStyleChange
	{
		bool HasChangeStyle			= false;

		bool HasSetIntensity		= false;
		bool HasSetItalic			= false;
		bool HasSetUnderlineColor	= false;
		bool HasSetUnderline		= false;
		bool HasSetBlink			= false;
		bool HasSetInverted			= false;
		bool HasSetIdeogram			= false;
		bool HasSetScript			= false;
	};


	const static inline ResetAnsiAllParameters	RESET_ANSI_ALL_PARAMETERS;
	const static inline ResetAnsiStyle			RESET_ANSI_STYLE;
	const static inline ResetAnsiUnderlineColor RESET_ANSI_UNDERLINE_COLOR;
}
