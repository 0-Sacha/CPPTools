#pragma once

#include "CPPTools/Core/Core.h"

#include "BaseColor.h"
#include "GetBaseType.h"

namespace CPPTools::Fmt::Detail {

	enum class ValueType : std::uint8_t {
		Param,
		Color,
		Time,
		Fail = 255
	};

	enum class ValueIntPrint : std::uint8_t {
		Int,
		Bin,
		Hex,
		Oct
	};

	enum class ShiftType : std::uint8_t {
		Nothing,
		Right,
		Left,
		Center
	};

	enum class ShiftPrint : std::uint8_t {
		Space,
		Zeros
	};

	enum class ContainerPrintStyle : std::uint8_t {
		CommaSpace,
		NewLine
	};
}

namespace CPPTools::Fmt {
	struct FormatSpecifierIDX {
		char Type = 0;
		std::int8_t Value = 0;
		std::uint8_t IdxType = 0;
	};

	struct FormatSpecifier {
		char Type = 0;
		std::int8_t Value = 0;
	};

	struct FormatData {
	public:
		FormatData();
		FormatData(const FormatData& other);
		FormatData& operator=(const FormatData& other);

		FormatData(bool hasSpec
			, bool baseValue = false
			, Detail::ValueIntPrint intPrint = Detail::ValueIntPrint::Int
			, std::int16_t precision = 0
			, std::int16_t floatPrecision = 3
			, std::int16_t size = -1
			, std::int16_t begin = 0
			, Detail::ContainerPrintStyle containerPrintStyle = Detail::ContainerPrintStyle::CommaSpace
			, Detail::ShiftPrint shiftPrint = Detail::ShiftPrint::Space
			, Detail::ShiftType shiftType = Detail::ShiftType::Nothing
			, std::uint8_t shiftValue = 0);

	public:
		void Clone(const FormatData& data);

	public:
		bool IsInit;
		bool HasSpec;
		bool BaseValue;

		Detail::ValueIntPrint IntPrint;
		std::int16_t Precision;
		std::int16_t FloatPrecision;

		std::int16_t Size;
		std::int16_t Begin;

		Detail::ContainerPrintStyle ContainerPrintStyle;

		Detail::ShiftPrint ShiftPrint;
		Detail::ShiftType ShiftType;
		std::int16_t ShiftValue;

		std::uint8_t SpecIdx;
		std::array<FormatSpecifier, 10> Spec;

	public:
		bool HasChangeColor;

	public:
		inline void SetDefaultSize(std::int16_t size)	{ if (Size == -1) Size = size; }
		inline void SetMaxSize(std::int16_t size)		{ if (Size == -1) SetDefaultSize(size); if(Begin + Size > size) Size = size - Begin; }

		const std::int8_t GetValueOf(const char c) const;
		inline void AddSpecifier(const FormatSpecifier specifier) {
			if (SpecIdx < Spec.size())
				Spec[SpecIdx++] = specifier;
		}
		inline void AddSpecifier(const char c, const std::int8_t value) {
			AddSpecifier(FormatSpecifier{ c, value });
		}
		static inline constexpr const std::int8_t NotFound() {
			return (std::numeric_limits<std::int8_t>::max)();
		}
	};
}

namespace CPPTools::Fmt::Detail {
	template<typename T>	struct CopyFormatData { static void Copy(FormatData& data, const T& t) { } };
	template<>				struct CopyFormatData<FormatData> { static void Copy(FormatData& data, const FormatData& t) { data = t; } };
}