#pragma once

#include "../BufferOut.h"

namespace CPPTools::Fmt::Detail {

	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteInt(T i) {
		if (i == 0) { PushBack('0'); return; }
		if (i < 0)	{ PushBack('-'); i = -i; }

		T i_ = i;
		int nb = 0;
		while (i_ > 0)		{ i_ /= 10; ++nb; }
		Forward(nb - 1);
		while (i > 0)		{ PushReverse(i % 10 + '0'); i /= 10; }
		Forward(nb + 1);
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteInt(T i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		T i_ = i < 0 ? -i : i;
		int nb = 0;

		while (i_ > 0)			{ i_ /= 10; ++nb; --shift; }
		if (i == 0)				--shift;
		if (i < 0)				--shift;
		
		if (st == Detail::ShiftType::Right) {
			if (sp == Detail::ShiftPrint::Space)		{ while (shift-- > 0) PushBack(' '); }
			else if (sp == Detail::ShiftPrint::Zeros)	{ while (shift-- > 0) PushBack('0'); }
		}

		if (i == 0) {
			--shift; PushBack('0');
			if (st == Detail::ShiftType::Left)	while (shift-- > 0) PushBack(' ');
			return;
		}

		if (i < 0)		{ PushBack('-'); i = -i; }
		Forward(nb - 1);
		while (i > 0)	{ PushReverse(i % 10 + '0'); i /= 10; }
		Forward(nb + 1);

		if (st == Detail::ShiftType::Left)		while (shift-- > 0) PushBack(' ');
	}


	//-------------------- Int Bin --------------------//

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsBin(T i, FormatDataType def) {
		if (def < 3)	def = sizeof(T) * 8;

		if (i < 0)		PushBack('1');
		else			PushBack('0');

		std::uint64_t mask = (std::uint64_t)1 << (def - 2);
		while (mask != 0) {
			if (i & mask)	PushBack('1');
			else			PushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsBin(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		if (def < 3)	def = sizeof(T) * 8;
		std::uint64_t mask = (std::uint64_t)1 << (def - 2);
		shift -= def;

		if (st == Detail::ShiftType::Right)		while (shift-- > 0) PushBack(' ');

		if (i < 0)	PushBack('1');
		else		PushBack('0');

		while (mask != 0) {
			if (i & mask)	PushBack('1');
			else			PushBack('0');
			mask = mask >> 1;
		}
		if (st == Detail::ShiftType::Left)		while (shift-- > 0)		PushBack(' ');
	}


	//-------------------- Int Hex --------------------//

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsHex(T i, FormatDataType def) {
		// static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsHex(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		// static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}


	//-------------------- Oct --------------------//

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsOct(T i, FormatDataType def) {}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteIntAsOct(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {}




	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//
	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUInt(T i) {
		if (i == 0) { PushBack('0'); return; }

		T i_ = i;
		int nb = 0;
		while (i_ > 0)			{ i_ /= 10; ++nb; }
		Forward(nb - 1);
		while (i > 0)			{ PushReverse(i % 10 + '0'); i /= 10; }
		Forward(nb + 1);
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUInt(T i, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		T i_ = i;
		std::int8_t nb = 0;
		while (i_ > 0)	{ i_ /= 10; ++nb; --shift; }
		if (i == 0)		--shift;

		if (st == Detail::ShiftType::Right) {
			if (sp == Detail::ShiftPrint::Space)		{ while (shift-- > 0) PushBack(' '); }
			else if (sp == Detail::ShiftPrint::Zeros)	{ while (shift-- > 0) PushBack('0'); }
		}

		if (i == 0) {
			PushBack('0');
			if (st == Detail::ShiftType::Left)		while (shift-- > 0) PushBack(' ');
			return;
		}

		Forward(nb - 1);
		while (i > 0) { PushReverse(i % 10 + '0'); i /= 10; }
		Forward(nb + 1);

		if (st == Detail::ShiftType::Left)
			while (shift-- > 0) PushBack(' ');
	}


	//-------------------- UInt Bin --------------------//

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsBin(T i, FormatDataType def) {
		if (def < 2)	def = sizeof(T) * 8;
		std::size_t mask = (std::size_t)1 << (def - 1);
		while (mask != 0) {
			if (i & mask)	PushBack('1');
			else			PushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsBin(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		if (def < 2)	def = sizeof(T) * 8;
		std::uint64_t mask = (std::uint64_t)1 << (def - 1);
		shift -= def;

		if (st == Detail::ShiftType::Right)		while (shift-- > 0) PushBack(' ');

		if (i < 0)	PushBack('1');
		else		PushBack('0');

		while (mask != 0) {
			if (i & mask)	PushBack('1');
			else			PushBack('0');
			mask = mask >> 1;
		}
		if (st == Detail::ShiftType::Left)
			while (shift-- > 0)			PushBack(' ');
	}


	//-------------------- UInt Hex --------------------//
	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsHex(T i, FormatDataType def) {
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
			PushBack(arr[k]);
			mask = mask >> 4;
		}
	}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsHex(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {
		// static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}



	//-------------------- UInt Oct --------------------//
	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsOct(T i, FormatDataType def) {}

	template<typename CharBuffer>
	template<typename T>
	void FormatterMemoryBufferOut<CharBuffer>::BasicWriteUIntAsOct(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp) {}
}