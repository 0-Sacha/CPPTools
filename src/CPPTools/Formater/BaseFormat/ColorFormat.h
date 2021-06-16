#pragma once

#include "FormatInclude.h"

namespace CPPTools::Fmt {

	template<>
	struct FormatType<Detail::ResetAnsiColor>
	{
		static void Write(const Detail::ResetAnsiColor t, Formater& formater) {
			formater.BufferPushBack('\033');
			formater.BufferPushBack('[');
			formater.BufferPushBack('m');

			formater.AddNoStride(3);
			

			formater.GetColorMem().IsSetColor = false;
		}

		static bool Read(const Detail::ResetAnsiColor t, UnFormater& formater) {
			return formater.BufferNextIsSame("\033[m");
		}
	};

	template<>
	struct FormatType<Detail::AnsiColorFG>
	{
		static void Write(const Detail::AnsiColorFG& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();

			formater.BufferPushBack('\033');
			formater.BufferPushBack('[');
			formater.BufferParseUInt<std::uint8_t>((std::uint8_t)t);
			formater.BufferPushBack('m');

			formater.AddNoStride(formater.GetCurrentBufferSize() - size);


			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor;
			formater.GetColorMem().Color.Fg = t;
		}

		static bool Read(Detail::AnsiColorFG& t, UnFormater& formater) {
			if (formater.BufferIsEqualForward('\033'))
				if (formater.BufferIsEqualForward('[')) {
					formater.BufferUnParseUInt<std::uint8_t>((std::uint8_t&)t);
					if (formater.BufferIsEqualForward('m'))
						return true;
				}
			return false;
		}
	};

	template<>
	struct FormatType<Detail::AnsiColorBG>
	{
		static void Write(const Detail::AnsiColorBG& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();

			formater.BufferPushBack('\033');
			formater.BufferPushBack('[');
			formater.BufferParseUInt<std::uint8_t>((std::uint8_t)t);
			formater.BufferPushBack('m');

			formater.AddNoStride(formater.GetCurrentBufferSize() - size);


			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor;
			formater.GetColorMem().Color.Bg = t;
		}

		static bool Read(Detail::AnsiColorBG& t, UnFormater& formater) {
			if (formater.BufferIsEqualForward('\033'))
				if (formater.BufferIsEqualForward('[')) {
					formater.BufferUnParseUInt<std::uint8_t>((std::uint8_t&)t);
					if (formater.BufferIsEqualForward('m'))	return true;
				}
			return false;
		}
	};

	template<>
	struct FormatType<Detail::AnsiColor>
	{
		static void Write(const Detail::AnsiColor& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();
			formater.LittleFormat("\033[{};{}m", (std::uint8_t)t.Fg, (std::uint8_t)t.Bg);
			formater.AddNoStride(formater.GetCurrentBufferSize() - size);

			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor;
			formater.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor;
			formater.GetColorMem().Color = t;
		}

		static bool Read(Detail::AnsiColor& t, UnFormater& formater) {
			return formater.LittleUnFormat("\033[{};{}m", (std::uint8_t)t.Fg, (std::uint8_t)t.Bg);
		}
	};

	template<>
	struct FormatType<Detail::AnsiColor24bFG>
	{
		static void Write(const Detail::AnsiColor24bFG& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();
			formater.LittleFormat("\033[38;2;{};{};{}m", t.R, t.G, t.B);
			formater.AddNoStride(formater.GetCurrentBufferSize() - size);

			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor24b;
			formater.GetColorMem().Color24bits.Fg = t;
		}

		static bool Read(Detail::AnsiColor24bFG& t, UnFormater& formater) {
			return formater.LittleUnFormat("\033[38;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<>
	struct FormatType<Detail::AnsiColor24bBG>
	{
		static void Write(const Detail::AnsiColor24bBG& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();
			formater.LittleFormat("\033[48;2;{};{};{}m", t.R, t.G, t.B);
			formater.AddNoStride(formater.GetCurrentBufferSize() - size);

			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor24b;
			formater.GetColorMem().Color24bits.Bg = t;
		}

		static bool Read(Detail::AnsiColor24bBG& t, UnFormater& formater) {
			return formater.LittleUnFormat("\033[48;2;{};{};{}m", t.R, t.G, t.B);
		}
	};

	template<>
	struct FormatType<Detail::AnsiColor24b>
	{
		static void Write(const Detail::AnsiColor24b& t, Formater& formater) {
			std::size_t size = formater.GetCurrentBufferSize();
			formater.LittleFormat("\033[38;2;{};{};{};48;2;{};{};{}m", t.Fg.R, t.Fg.G, t.Fg.B, t.Bg.R, t.Bg.G, t.Bg.B);
			formater.AddNoStride(formater.GetCurrentBufferSize() - size);

			formater.GetColorMem().IsSetColor = true;
			formater.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor24b;
			formater.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor24b;
			formater.GetColorMem().Color24bits = t;
		}

		static bool Read(Detail::AnsiColor24b& t, UnFormater& formater) {
			return formater.LittleUnFormat("\033[38;2;{};{};{};48;2;{};{};{}m", t.Fg.R, t.Fg.G, t.Fg.B, t.Bg.R, t.Bg.G, t.Bg.B);
		}
	};
}