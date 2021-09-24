#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColorFG, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColorFG& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color.Fg = t;

			return context.BasicReadType('\033', '[', dynamic_cast<std::uint8_t&>(t), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColorBG, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColorBG& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color.Bg = t;

			return context.BasicReadType('\033', '[', dynamic_cast<std::uint8_t&>(t), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColor, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColor& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor;
			context.GetAnsiTextCurrentColor().Color = t;

			return context.BasicReadType('\033', '[', dynamic_cast<std::uint8_t&>(t.Fg), ';'
													, dynamic_cast<std::uint8_t&>(t.Bg), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColor24bFg, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColor24bFg& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits.Fg = t;

			return context.BasicReadType("\033[38;2;", t.R, ';', t.G, ';', t.B, 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColor24bBg, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColor24bBg& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits.Bg = t;

			return context.BasicReadType("\033[48;2;", t.R, ';', t.G, ';', t.B, 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextColor24b, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextColor24b& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextColor24b;
			context.GetAnsiTextCurrentColor().Color24bits = t;

			return context.BasicReadType("\033[38;2;", t.Fg.R, ';', t.Fg.G, ';', t.Fg.B, "; 48; 2;", t.Bg.R, ';', t.Bg.G, ';', t.Bg.B, 'm');
		}
	};


	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextNColorFg, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextNColorFg& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN.Fg = t;

			return context.BasicReadType("\033[38;5;", t.GetColorRef(), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextNColorBg, UnFormatContext>
	{
		static bool Read(Detail::AnsiTextNColorBg& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN.Bg = t;

			return context.BasicReadType("\033[48;5;", t.GetColorRef(), 'm');
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextNColor, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTextNColor& t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.GetFormatData().AnsiTextColorChange.HasSetFg = true;
			context.GetFormatData().AnsiTextColorChange.HasSetBg = true;
			context.GetAnsiTextCurrentColor().FgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().BgType = Detail::AnsiTextColorDataType::AnsiTextNColor;
			context.GetAnsiTextCurrentColor().ColorN = t;

			return context.BasicReadType("\033[48;5;", t.Fg.GetColorRef(), ";48;5;", t.Bg.GetColorRef(), 'm');
		}
	};

}