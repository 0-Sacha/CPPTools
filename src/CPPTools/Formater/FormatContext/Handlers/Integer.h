#pragma once

#include "../BasicFormatContext.h"

namespace CPPTools::Fmt::FormatFunc {

	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T, typename Char>
	void FormatContextWriteInt(BasicFormatContext<Char>& formater, T i) {
		if (i == 0) { formater.BufferPushBack('0'); return; }
		if (i < 0)	{ formater.BufferPushBack('-'); i = -i; }

		T i_ = i;
		std::int8_t nb = 0;
		while (i_ > 0)		{ i_ /= 10; ++nb; }
		formater.BufferForward((std::size_t)(nb - 1));
		while (i > 0)		{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferForward((std::size_t)(nb + 1));
	}

	template<typename T, typename Char>
	void FormatContextWriteInt(BasicFormatContext<Char>& formater, T i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		T i_ = i < 0 ? -i : i;
		std::int8_t nb = 0;

		while (i_ > 0)			{ i_ /= 10; ++nb; --shift; }
		if (i == 0)				--shift;
		if (i < 0)				--shift;
		
		if (st == Detail::ShiftType::Right) {
			if (sp == Detail::ShiftPrint::Space)		{ while (shift-- > 0) formater.BufferPushBack(' '); }
			else if (sp == Detail::ShiftPrint::Zeros)	{ while (shift-- > 0) formater.BufferPushBack('0'); }
		}

		if (i == 0) {
			--shift; formater.BufferPushBack('0');
			if (st == Detail::ShiftType::Left)	while (shift-- > 0) formater.BufferPushBack(' ');
			return;
		}

		if (i < 0)		{ formater.BufferPushBack('-'); i = -i; }
		formater.BufferForward((std::size_t)(nb - 1));
		while (i > 0)	{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferForward((std::size_t)(nb + 1));

		if (st == Detail::ShiftType::Left)		while (shift-- > 0) formater.BufferPushBack(' ');
	}


	//-------------------- Int Bin --------------------//

	template<typename T, typename Char>
	void FormatContextWriteIntAsBin(BasicFormatContext<Char>& formater, T i, std::int16_t def) {
		if (def < 3)	def = sizeof(T) * 8;

		if (i < 0)		formater.BufferPushBack('1');
		else			formater.BufferPushBack('0');

		std::uint64_t mask = (std::uint64_t)1 << (def - 2);
		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename T, typename Char>
	void FormatContextWriteIntAsBin(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		if (def < 3)	def = sizeof(T) * 8;
		std::uint64_t mask = (std::uint64_t)1 << (def - 2);
		shift -= def;

		if (st == Detail::ShiftType::Right)		while (shift-- > 0) formater.BufferPushBack(' ');

		if (i < 0)	formater.BufferPushBack('1');
		else		formater.BufferPushBack('0');

		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
		if (st == Detail::ShiftType::Left)		while (shift-- > 0)		formater.BufferPushBack(' ');
	}


	//-------------------- Int Hex --------------------//

	template<typename T, typename Char>
	void FormatContextWriteIntAsHex(BasicFormatContext<Char>& formater, T i, std::int16_t def) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}

	template<typename T, typename Char>
	void FormatContextWriteIntAsHex(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}


	//-------------------- Oct --------------------//

	template<typename T, typename Char>
	void FormatContextWriteIntAsOct(BasicFormatContext<Char>& formater, T i, std::int16_t def) {}

	template<typename T, typename Char>
	void FormatContextWriteIntAsOct(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {}




	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T, typename Char>
	void FormatContextWriteUInt(BasicFormatContext<Char>& formater, T i) {
		if (i == 0) { formater.BufferPushBack('0'); return; }

		T i_ = i;
		int nb = 0;
		while (i_ > 0)			{ i_ /= 10; ++nb; }
		formater.BufferForward((std::size_t)(nb - 1));
		while (i > 0)			{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferForward((std::size_t)(nb + 1));
	}

	template<typename T, typename Char>
	void FormatContextWriteUInt(BasicFormatContext<Char>& formater, T i, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		T i_ = i;
		std::int8_t nb = 0;
		while (i_ > 0)	{ i_ /= 10; ++nb; --shift; }
		if (i == 0)		--shift;

		if (st == Detail::ShiftType::Right) {
			if (sp == Detail::ShiftPrint::Space)		{ while (shift-- > 0) formater.BufferPushBack(' '); }
			else if (sp == Detail::ShiftPrint::Zeros)	{ while (shift-- > 0) formater.BufferPushBack('0'); }
		}

		if (i == 0) {
			formater.BufferPushBack('0');
			if (st == Detail::ShiftType::Left)		while (shift-- > 0) formater.BufferPushBack(' ');
			return;
		}

		formater.BufferForward((std::size_t)(nb - 1));
		while (i > 0) { formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferForward((std::size_t)(nb + 1));

		if (st == Detail::ShiftType::Left)
			while (shift-- > 0) formater.BufferPushBack(' ');
	}


	//-------------------- UInt Bin --------------------//

	template<typename T, typename Char>
	void FormatContextWriteUIntAsBin(BasicFormatContext<Char>& formater, T i, std::int16_t def) {
		if (def < 2)	def = sizeof(T) * 8;
		std::size_t mask = (std::size_t)1 << (def - 1);
		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename T, typename Char>
	void FormatContextWriteUIntAsBin(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		if (def < 2)	def = sizeof(T) * 8;
		T i_ = i;
		std::uint64_t mask = (std::uint64_t)1 << (def - 1);
		shift -= def;

		if (st == Detail::ShiftType::Right)		while (shift-- > 0) formater.BufferPushBack(' ');

		if (i < 0)	formater.BufferPushBack('1');
		else		formater.BufferPushBack('0');

		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
		if (st == Detail::ShiftType::Left)
			while (shift-- > 0)			formater.BufferPushBack(' ');
	}


	//-------------------- UInt Hex --------------------//

	template<typename T, typename Char>
	void FormatContextWriteUIntAsHex(BasicFormatContext<Char>& formater, T i, std::int16_t def) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		std::size_t mask = (std::size_t)0b1111;

		if (def == 0) {
			T i_ = i;
			while (i_ != 0) { i_ -= i_ & mask; mask = mask << 4; def++; }
			mask = mask >> 4;
		}
		else { mask = mask << (def - 1) * 4; }

		--def;
		while (mask != 0) {
			std::size_t k = ((std::size_t)i & mask) >> (def--) * 4;
			formater.BufferPushBack(arr[k]);
			mask = mask >> 4;
		}
	}

	template<typename T, typename Char>
	void FormatContextWriteUIntAsHex(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}



	//-------------------- UInt Oct --------------------//

	template<typename T, typename Char>
	void FormatContextWriteUIntAsOct(BasicFormatContext<Char>& formater, T i, std::int16_t def) {}

	template<typename T, typename Char>
	void FormatContextWriteUIntAsOct(BasicFormatContext<Char>& formater, T i, std::int16_t def, Detail::ShiftType st, std::int32_t shift, Detail::ShiftPrint sp = Detail::ShiftPrint::Space) {}
}