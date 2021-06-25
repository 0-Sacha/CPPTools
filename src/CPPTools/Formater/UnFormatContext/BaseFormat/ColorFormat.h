#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<Detail::ResetAnsiColor, UnFormatContext>
	{
		static bool Read(const Detail::ResetAnsiColor t, UnFormatContext& context) {
			return context.BufferNextIsSame("\033[m");
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColorFG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColorFG& t, UnFormatContext& context) {
			if (context.BufferIsEqualForward('\033'))
				if (context.BufferIsEqualForward('[')) {
					context.BufferReadUInt((std::uint8_t&)t);
					if (context.BufferIsEqualForward('m'))
						return true;
				}
			return false;
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColorBG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColorBG& t, UnFormatContext& context) {
			if (context.BufferIsEqualForward('\033'))
				if (context.BufferIsEqualForward('[')) {
					context.BufferReadUInt((std::uint8_t&)t);
					if (context.BufferIsEqualForward('m'))	return true;
				}
			return false;
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor& t, UnFormatContext& context) {
			return context.LittleUnFormat("\033[{};{}m", (std::uint8_t)t.Fg, (std::uint8_t)t.Bg);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24bFG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24bFG& t, UnFormatContext& context) {
			return context.LittleUnFormat("\033[38;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24bBG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24bBG& t, UnFormatContext& context) {
			return context.LittleUnFormat("\033[48;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24b, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24b& t, UnFormatContext& context) {
			return context.LittleUnFormat("\033[38;2;{};{};{};48;2;{};{};{}m", t.Fg.R, t.Fg.G, t.Fg.B, t.Bg.R, t.Bg.G, t.Bg.B);
		}
	};
}