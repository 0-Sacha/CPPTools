#pragma once

#include "BasicBufferIn.h"

namespace CPPTools::Fmt::Detail {

	template<typename CharBuffer>
	class FormatterMemoryBufferIn : public BasicFormatterMemoryBufferIn<CharBuffer> {

	public:
		using CharBufferType = CharBuffer;

	public:
		FormatterMemoryBufferIn(const std::basic_string_view<CharBuffer> format)
			: BasicFormatterMemoryBufferIn<CharBuffer>(format) {}

	public:
		// TODO
		template <typename ChildBuffer>
		inline void UpdateFromChlidBuffer(ChildBuffer& childBuffer) { SetBufferCurrentPos(childBuffer.GetBufferCurrentPos()); }

	public:
		template<typename T> bool BasicReadInt(T& i);
		template<typename T> bool BasicReadInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadIntAsBin(T& i, FormatDataType def);
		template<typename T> bool BasicReadIntAsBin(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadIntAsHex(T& i, FormatDataType def);
		template<typename T> bool BasicReadIntAsHex(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadIntAsOct(T& i, FormatDataType def);
		template<typename T> bool BasicReadIntAsOct(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);

		template<typename T> bool BasicReadUInt(T& i);
		template<typename T> bool BasicReadUInt(T& i, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadUIntAsBin(T& i, FormatDataType def);
		template<typename T> bool BasicReadUIntAsBin(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadUIntAsHex(T& i, FormatDataType def);
		template<typename T> bool BasicReadUIntAsHex(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> bool BasicReadUIntAsOct(T& i, FormatDataType def);
		template<typename T> bool BasicReadUIntAsOct(T& i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);


		template<typename T> bool BasicReadFloat(T& i, FormatDataType nbDecimal);
		template<typename T> bool BasicReadFloat(T& i, FormatDataType nbDecimal, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);


	public:
		template<typename T, typename FormatDataCharType> bool ReadIntFormatData(T& i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> bool ReadUIntFormatData(T& i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> bool ReadFloatFormatData(T& i, const FormatData<FormatDataCharType>& formatData);
	};

}