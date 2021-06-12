#pragma once

#include "../Formater.h"

namespace CPPTools::Fmt::FormatFunc {

	//---------------------------------------------//
	//-------------------- Int --------------------//
	//---------------------------------------------//

	template<typename T>
	void FormaterParseInt(Formater& formater, T i) {
		if (i == 0) { formater.BufferPushBack('0'); return; }
		if (i < 0)	{ formater.BufferPushBack('-'); i = -i; }

		T i_ = i;
		int8_t nb = 0;
		while (i_ > 0)		{ i_ /= 10; ++nb; }
		formater.BufferShiftAdd(nb - 1);
		while (i > 0)		{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferShiftAdd(nb + 1);
	}

	template<typename T>
	void FormaterParseInt(Formater& formater, T i, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		T i_ = i < 0 ? -i : i;
		int8_t nb = 0;

		while (i_ > 0)			{ i_ /= 10; ++nb; --shift; }
		if (i == 0)				--shift;
		if (i < 0)				--shift;
		
		if (st == ShiftType::Right) {
			if (sp == ShiftPrint::Space)		{ while (shift-- > 0) formater.BufferPushBack(' '); }
			else if (sp == ShiftPrint::Zeros)	{ while (shift-- > 0) formater.BufferPushBack('0'); }
		}

		if (i == 0) {
			--shift; formater.BufferPushBack('0');
			if (st == ShiftType::Left)	while (shift-- > 0) formater.BufferPushBack(' ');
			return;
		}

		if (i < 0)		{ formater.BufferPushBack('-'); i = -i; }
		formater.BufferShiftAdd(nb - 1);
		while (i > 0)	{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferShiftAdd(nb + 1);

		if (st == ShiftType::Left)		while (shift-- > 0) formater.BufferPushBack(' ');
	}


	//-------------------- Int Bin --------------------//

	template<typename T>
	void FormaterParseIntAsBin(Formater& formater, T i, int8_t def) {
		if (def < 3)	def = sizeof(T) * 8;

		if (i < 0)		formater.BufferPushBack('1');
		else			formater.BufferPushBack('0');

		uint64_t mask = (uint64_t)1 << (def - 2);
		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename T>
	void FormaterParseIntAsBin(Formater& formater, T i, int8_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		if (def < 3)	def = sizeof(T) * 8;
		uint64_t mask = (uint64_t)1 << (def - 2);
		shift -= def;

		if (st == ShiftType::Right)		while (shift-- > 0) formater.BufferPushBack(' ');

		if (i < 0)	formater.BufferPushBack('1');
		else		formater.BufferPushBack('0');

		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
		if (st == ShiftType::Left)		while (shift-- > 0)		formater.BufferPushBack(' ');
	}


	//-------------------- Int Hex --------------------//

	template<typename T>
	void FormaterParseIntAsHex(Formater& formater, T i, int8_t def) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}

	template<typename T>
	void FormaterParseIntAsHex(Formater& formater, T i, int8_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}


	//-------------------- Oct --------------------//

	template<typename T>
	void FormaterParseIntAsOct(Formater& formater, T i, int8_t def) {}

	template<typename T>
	void FormaterParseIntAsOct(Formater& formater, T i, int8_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {}




	//----------------------------------------------//
	//-------------------- UInt --------------------//
	//----------------------------------------------//

	template<typename T>
	void FormaterParseUInt(Formater& formater, T i) {
		if (i == 0) { formater.BufferPushBack('0'); return; }

		T i_ = i;
		int nb = 0;
		while (i_ > 0)			{ i_ /= 10; ++nb; }
		formater.BufferShiftAdd(nb - 1);
		while (i > 0)			{ formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferShiftAdd(nb + 1);
	}

	template<typename T>
	void FormaterParseUInt(Formater& formater, T i, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		T i_ = i;
		int8_t nb = 0;
		while (i_ > 0)	{ i_ /= 10; ++nb; --shift; }
		if (i == 0)		--shift;

		if (st == ShiftType::Right) {
			if (sp == ShiftPrint::Space)		{ while (shift-- > 0) formater.BufferPushBack(' '); }
			else if (sp == ShiftPrint::Zeros)	{ while (shift-- > 0) formater.BufferPushBack('0'); }
		}

		if (i == 0) {
			formater.BufferPushBack('0');
			if (st == ShiftType::Left)		while (shift-- > 0) formater.BufferPushBack(' ');
			return;
		}

		formater.BufferShiftAdd(nb - 1);
		while (i > 0) { formater.BufferPushReverse(i % 10 + '0'); i /= 10; }
		formater.BufferShiftAdd(nb + 1);

		if (st == ShiftType::Left)
			while (shift-- > 0) formater.BufferPushBack(' ');
	}


	//-------------------- UInt Bin --------------------//

	template<typename T>
	void FormaterParseUIntAsBin(Formater& formater, T i, int16_t def) {
		if (def < 2)	def = sizeof(T) * 8;
		size_t mask = (size_t)1 << (def - 1);
		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
	}

	template<typename T>
	void FormaterParseUIntAsBin(Formater& formater, T i, int16_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		if (def < 2)	def = sizeof(T) * 8;
		T i_ = i;
		uint64_t mask = (uint64_t)1 << (def - 1);
		shift -= def;

		if (st == ShiftType::Right)		while (shift-- > 0) formater.BufferPushBack(' ');

		if (i < 0)	formater.BufferPushBack('1');
		else		formater.BufferPushBack('0');

		while (mask != 0) {
			if (i & mask)	formater.BufferPushBack('1');
			else			formater.BufferPushBack('0');
			mask = mask >> 1;
		}
		if (st == ShiftType::Left)
			while (shift-- > 0)			formater.BufferPushBack(' ');
	}


	//-------------------- UInt Hex --------------------//

	template<typename T>
	void FormaterParseUIntAsHex(Formater& formater, T i, int16_t def) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		size_t mask = (size_t)0b1111;

		if (def == 0) {
			T i_ = i;
			while (i_ != 0) { i_ -= i_ & mask; mask = mask << 4; def++; }
			mask = mask >> 4;
		}
		else { mask = mask << (def - 1) * 4; }

		--def;
		while (mask != 0) {
			size_t k = ((size_t)i & mask) >> (def--) * 4;
			formater.BufferPushBack(arr[k]);
			mask = mask >> 4;
		}
	}

	template<typename T>
	void FormaterParseUIntAsHex(Formater& formater, T i, int16_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {
		static char arr[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	}



	//-------------------- UInt Oct --------------------//

	template<typename T>
	void FormaterParseUIntAsOct(Formater& formater, T i, int16_t def) {}

	template<typename T>
	void FormaterParseUIntAsOct(Formater& formater, T i, int16_t def, ShiftType st, int32_t shift, ShiftPrint sp = ShiftPrint::Space) {}
}