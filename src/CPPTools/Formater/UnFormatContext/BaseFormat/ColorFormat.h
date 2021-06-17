#pragma once

#include "../BasicUnFormatContextClassImpl.h"

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<Detail::ResetAnsiColor, UnFormatContext>
	{
		static bool Read(const Detail::ResetAnsiColor t, UnFormatContext& formater) {
			return formater.BufferNextIsSame("\033[m");
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColorFG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColorFG& t, UnFormatContext& formater) {
			if (formater.BufferIsEqualForward('\033'))
				if (formater.BufferIsEqualForward('[')) {
					formater.BufferUnParseUInt<std::uint8_t>((std::uint8_t&)t);
					if (formater.BufferIsEqualForward('m'))
						return true;
				}
			return false;
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColorBG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColorBG& t, UnFormatContext& formater) {
			if (formater.BufferIsEqualForward('\033'))
				if (formater.BufferIsEqualForward('[')) {
					formater.BufferUnParseUInt<std::uint8_t>((std::uint8_t&)t);
					if (formater.BufferIsEqualForward('m'))	return true;
				}
			return false;
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor& t, UnFormatContext& formater) {
			return formater.LittleUnFormat("\033[{};{}m", (std::uint8_t)t.Fg, (std::uint8_t)t.Bg);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24bFG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24bFG& t, UnFormatContext& formater) {
			return formater.LittleUnFormat("\033[38;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24bBG, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24bBG& t, UnFormatContext& formater) {
			return formater.LittleUnFormat("\033[48;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiColor24b, UnFormatContext>
	{
		static bool Read(Detail::AnsiColor24b& t, UnFormatContext& formater) {
			return formater.LittleUnFormat("\033[38;2;{};{};{};48;2;{};{};{}m", t.Fg.R, t.Fg.G, t.Fg.B, t.Bg.R, t.Bg.G, t.Bg.B);
		}
	};
}