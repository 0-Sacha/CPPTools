#pragma once

#include "../BasicFormatContext.h"

// According to : https://en.wikipedia.org/wiki/ANSI_escape_code

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColorFG, FormatContext>
	{
		static void Write(const Detail::AnsiTextColorFG& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType('\033', '[', static_cast<std::uint8_t>(t), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg		= true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color.Fg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColorBG, FormatContext>
	{
		static void Write(const Detail::AnsiTextColorBG& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType('\033', '[', static_cast<std::uint8_t>(t), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color.Bg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColor, FormatContext>
	{
		static void Write(const Detail::AnsiTextColor& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType('\033', '[', static_cast<std::uint8_t>(t.Fg)
										 , ';', static_cast<std::uint8_t>(t.Bg), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColor24bFg, FormatContext>
	{
		static void Write(const Detail::AnsiTextColor24bFg& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[38;2;", t.R, ';', t.G, ';', t.B, 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits.Fg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColor24bBg, FormatContext>
	{
		static void Write(const Detail::AnsiTextColor24bBg& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[48;2;", t.R, ';', t.G, ';', t.B, 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits.Bg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextColor24b, FormatContext>
	{
		static void Write(const Detail::AnsiTextColor24b& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[38;2;", t.Fg.R, ';', t.Fg.G, ';', t.Fg.B, "; 48; 2;", t.Bg.R, ';', t.Bg.G, ';', t.Bg.B, 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits = t;
		}
	};


	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextNColorFg, FormatContext>
	{
		static void Write(const Detail::AnsiTextNColorFg& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[38;5;", t.GetColorRef(), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN.Fg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextNColorBg, FormatContext>
	{
		static void Write(const Detail::AnsiTextNColorBg& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[48;5;", t.GetColorRef(), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN.Bg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextNColor, FormatContext>
	{
		static void Write(const Detail::AnsiTextNColor& t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType("\033[48;5;", t.Fg.GetColorRef(), ";48;5;", t.Bg.GetColorRef(), 'm');

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN = t;
		}
	};

}