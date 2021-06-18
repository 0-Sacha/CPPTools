#pragma once

#include "../BasicFormatContextImpl.h"

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<Detail::ResetAnsiColor, FormatContext>
	{
		static void Write(const Detail::ResetAnsiColor t, FormatContext& context) {
			context.BufferPushBack('\033');
			context.BufferPushBack('[');
			context.BufferPushBack('m');

			context.AddNoStride(3);
			

			context.GetColorMem().IsSetColor = false;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColorFG, FormatContext>
	{
		static void Write(const Detail::AnsiColorFG& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();

			context.BufferPushBack('\033');
			context.BufferPushBack('[');
			context.BufferWriteUInt<std::uint8_t>((std::uint8_t)t);
			context.BufferPushBack('m');

			context.AddNoStride(context.GetCurrentBufferSize() - size);


			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor;
			context.GetColorMem().Color.Fg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColorBG, FormatContext>
	{
		static void Write(const Detail::AnsiColorBG& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();

			context.BufferPushBack('\033');
			context.BufferPushBack('[');
			context.BufferWriteUInt<std::uint8_t>((std::uint8_t)t);
			context.BufferPushBack('m');

			context.AddNoStride(context.GetCurrentBufferSize() - size);


			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor;
			context.GetColorMem().Color.Bg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColor, FormatContext>
	{
		static void Write(const Detail::AnsiColor& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();
			context.LittleFormat("\033[{};{}m", (std::uint8_t)t.Fg, (std::uint8_t)t.Bg);
			context.AddNoStride(context.GetCurrentBufferSize() - size);

			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor;
			context.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor;
			context.GetColorMem().Color = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColor24bFG, FormatContext>
	{
		static void Write(const Detail::AnsiColor24bFG& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();
			context.LittleFormat("\033[38;2;{};{};{}m", t.R, t.G, t.B);
			context.AddNoStride(context.GetCurrentBufferSize() - size);

			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor24b;
			context.GetColorMem().Color24bits.Fg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColor24bBG, FormatContext>
	{
		static void Write(const Detail::AnsiColor24bBG& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();
			context.LittleFormat("\033[48;2;{};{};{}m", t.R, t.G, t.B);
			context.AddNoStride(context.GetCurrentBufferSize() - size);

			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor24b;
			context.GetColorMem().Color24bits.Bg = t;
		}
	};

	template<typename FormatContext>
	struct FormatType<Detail::AnsiColor24b, FormatContext>
	{
		static void Write(const Detail::AnsiColor24b& t, FormatContext& context) {
			std::size_t size = context.GetCurrentBufferSize();
			context.LittleFormat("\033[38;2;{};{};{};48;2;{};{};{}m", t.Fg.R, t.Fg.G, t.Fg.B, t.Bg.R, t.Bg.G, t.Bg.B);
			context.AddNoStride(context.GetCurrentBufferSize() - size);

			context.GetColorMem().IsSetColor = true;
			context.GetColorMem().FgType = Detail::AnsiColorFGType::AnsiColor24b;
			context.GetColorMem().BgType = Detail::AnsiColorBGType::AnsiColor24b;
			context.GetColorMem().Color24bits = t;
		}
	};
}