#pragma once

#include "CPPTools/Core/Core.h"

#include "BaseColor.h"
#include "GetBaseType.h"

namespace CPPTools::Fmt {
	using FormatDataType = std::int16_t;
} // namespace CPPTools::Fmt

namespace CPPTools::Fmt::Detail {

	enum class ValueType : FormatDataType {
		Param,
		Color,
		Time,
		Fail = 255
	};

	enum class ValueIntPrint : FormatDataType {
		Int,
		Bin,
		Hex,
		Oct
	};

	enum class ShiftType : FormatDataType {
		Nothing,
		Right,
		Left,
		Center
	};

	enum class ShiftPrint : FormatDataType {
		Space,
		Zeros
	};

	enum class ContainerPrintStyle : FormatDataType {
		CommaSpace,
		NewLine
	};
} // CPPTools::Fmt::Detail

namespace CPPTools::Fmt {

	using FormatIdx = int;
	static const FormatIdx FormatIdxNotFound = -1;

	struct FormatSpecifier {
		char		Type = 0;
		std::int8_t Value = 0;
	};

	struct FormatData {
	public:
		explicit FormatData();
		explicit FormatData(const FormatData& other);
		FormatData& operator=(const FormatData& other);

		explicit FormatData(bool hasSpec
			, bool baseValue = false
			, Detail::ValueIntPrint intPrint = Detail::ValueIntPrint::Int
			, FormatDataType precision = 0
			, FormatDataType floatPrecision = 3
			, FormatDataType size = -1
			, FormatDataType begin = 0
			, Detail::ContainerPrintStyle containerPrintStyle = Detail::ContainerPrintStyle::CommaSpace
			, Detail::ShiftPrint shiftPrint = Detail::ShiftPrint::Space
			, Detail::ShiftType shiftType = Detail::ShiftType::Nothing
			, FormatDataType shiftValue = 0);

	public:
		void Clone(const FormatData& other);

	public:
		bool IsInit;
		bool HasSpec;
		bool BaseValue;

		Detail::ValueIntPrint IntPrint;
		FormatDataType Precision;
		FormatDataType FloatPrecision;

		FormatDataType Size;
		FormatDataType Begin;

		Detail::ContainerPrintStyle ContainerPrintStyle;

		Detail::ShiftPrint ShiftPrint;
		Detail::ShiftType ShiftType;
		FormatDataType ShiftValue;

		std::uint8_t SpecIdx;
		std::array<FormatSpecifier, 10> Spec;

	public:
		bool HasChangeColor;

	public:
		inline void SetDefaultSize(FormatDataType size)	{ if (Size == -1) Size = size; }
		inline void SetMaxSize(FormatDataType size)		{ if (Size == -1) SetDefaultSize(size); if(Begin + Size > size) Size = size - Begin; }

		std::int8_t GetValueOf(const char c) const;
		inline void AddSpecifier(const FormatSpecifier specifier) {
			if (SpecIdx < Spec.size())
				Spec[SpecIdx++] = specifier;
		}
		inline void AddSpecifier(const char c, const std::int8_t value) {
			AddSpecifier(FormatSpecifier{ c, value });
		}
		static inline constexpr std::int8_t NotFound() {
			return (std::numeric_limits<std::int8_t>::max)();
		}
	};
} // CPPTools::Fmt

namespace CPPTools::Fmt::Detail {
	template<typename T>	struct CopyFormatData				{ static void Copy(FormatData& data, const T& t) { } };
	template<>				struct CopyFormatData<FormatData>	{ static void Copy(FormatData& data, const FormatData& t) { data = t; } };
} // CPPTools::Fmt::Detail