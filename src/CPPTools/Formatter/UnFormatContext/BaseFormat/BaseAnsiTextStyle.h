#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {


	template<typename UnFormatContext>
	struct UnFormatType<Detail::ResetAnsiAllParameters, UnFormatContext>
	{
		static bool Read(const Detail::ResetAnsiAllParameters& t, UnFormatContext& context) {
			context.AddNoStride(3);
			return context.BasicReadType('\033', '[', 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::ResetAnsiStyle, UnFormatContext>
	{
		static bool Read(const Detail::ResetAnsiStyle& t, UnFormatContext& context) {
			return false;
		}
	};

	// Ansi text format style

	template<typename UnFormatContext>
	struct UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>
	{
		static bool Read(const Detail::ForwardAsAnsiBasicTextStyle& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			return context.BasicReadType('\033', '[', dynamic_cast<std::uint8_t&>(t), 'm');
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSIntensity, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSIntensity& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetIntensity = true;
			context.GetAnsiTextCurrentStyle().Intensity					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSItalic, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSItalic& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetItalic	= true;
			context.GetAnsiTextCurrentStyle().Italic					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSUnderline, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSUnderline& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetUnderline = true;
			context.GetAnsiTextCurrentStyle().Underline					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::ResetAnsiUnderlineColor, UnFormatContext>
	{
		static bool Read(const Detail::ResetAnsiUnderlineColor& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetUnderlineColor = true;

			return context.BufferOut().WriteCharArray("\033[59m");
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiNColorUnderline, UnFormatContext>
	{
		static bool Read(const Detail::AnsiNColorUnderline& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextStyleChange.HasSetUnderlineColor	= true;
			context.GetAnsiTextCurrentStyle().UnderlineColorType				= Detail::AnsiColorUnderlineType::AnsiTextNColor;
			context.GetAnsiTextCurrentStyle().UnderlineColorN					= t;

			return context.BasicReadType("\033[58;5;", t.GetColorRef(), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiUnderlineColor24b, UnFormatContext>
	{
		static bool Read(const Detail::AnsiUnderlineColor24b& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextStyleChange.HasSetUnderlineColor	= true;
			context.GetAnsiTextCurrentStyle().UnderlineColorType				= Detail::AnsiColorUnderlineType::AnsiTextColor24b;
			context.GetAnsiTextCurrentStyle().UnderlineColor24bits				= t;

			return context.BasicReadType("\033[58;2;", t.R, ';', t.G, ';', t.B, 'm');
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSBlink, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSBlink& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetBlink	= true;
			context.GetAnsiTextCurrentStyle().Blink					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSInverted, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSInverted& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetInverted	= true;
			context.GetAnsiTextCurrentStyle().Inverted					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSIdeogram, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSIdeogram& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetIdeogram	= true;
			context.GetAnsiTextCurrentStyle().Ideogram					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};



	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTFSScript, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTFSScript& t, UnFormatContext& context) {
			context.GetFormatData().AnsiTextStyleChange.HasSetScript	= true;
			context.GetAnsiTextCurrentStyle().Script					= t;

			return UnFormatType<Detail::ForwardAsAnsiBasicTextStyle, UnFormatContext>::Read(dynamic_cast<Detail::ForwardAsAnsiBasicTextStyle&>(t), context);
		}
	};


	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiBasicTextStyle, UnFormatContext>
	{
		static bool Read(const Detail::AnsiBasicTextStyle& t, UnFormatContext& context) {
			switch (t) {
			case Detail::AnsiBasicTextStyle::Intensity_Bold:
			case Detail::AnsiBasicTextStyle::Intensity_Dim:
			case Detail::AnsiBasicTextStyle::Intensity_Normal:
				return UnFormatType<Detail::AnsiTFSIntensity, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSIntensity&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Italic_Enable:
			case Detail::AnsiBasicTextStyle::Italic_Disable:
				return UnFormatType<Detail::AnsiTFSItalic, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSItalic&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Underline_Underlined:
			case Detail::AnsiBasicTextStyle::Underline_DoubleUnerlined:
			case Detail::AnsiBasicTextStyle::Underline_Disable:
				return UnFormatType<Detail::AnsiTFSUnderline, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSUnderline&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Blink_SlowBlink:
			case Detail::AnsiBasicTextStyle::Blink_FastBlink:
			case Detail::AnsiBasicTextStyle::Blink_Disable:
				return UnFormatType<Detail::AnsiTFSBlink, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSBlink&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Inverted_Enable:
			case Detail::AnsiBasicTextStyle::Inverted_Disable:
				return UnFormatType<Detail::AnsiTFSInverted, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSInverted&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Ideogram_Underlined:
			case Detail::AnsiBasicTextStyle::Ideogram_DoubleUnderlined:
			case Detail::AnsiBasicTextStyle::Ideogram_Overlined:
			case Detail::AnsiBasicTextStyle::Ideogram_DoubleOverlined:
			case Detail::AnsiBasicTextStyle::Ideogram_StressMarking:
			case Detail::AnsiBasicTextStyle::Ideogram_AllDisable:
				return UnFormatType<Detail::AnsiTFSIdeogram, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSIdeogram&>(t), context);
				break;

			case Detail::AnsiBasicTextStyle::Script_Superscript:
			case Detail::AnsiBasicTextStyle::Script_Subscript:
			case Detail::AnsiBasicTextStyle::Script_AllDisable:
				return UnFormatType<Detail::AnsiTFSScript, UnFormatContext>::Read(dynamic_cast<Detail::AnsiTFSScript&>(t), context);
				break;
			}
		}
	};
}