#pragma once

#include "CPPTools/Core/Core.h"

#include "BaseColor.h"
#include "GetBaseType.h"

namespace CPPTools::Fmt {

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
		FormatData(bool baseValue, ValueIntPrint intPrint = ValueIntPrint::Int, std::uint8_t precision = 0, std::uint8_t floatPrecision = 3, ShiftPrint shiftPrint = ShiftPrint::Space, ShiftType shiftType = ShiftType::Nothing,  std::uint8_t shiftValue = 0);
		FormatData(const FormatData& other);
		FormatData& operator=(const FormatData& other);

	public:
		void Clone(const FormatData& data);

	public:
		bool IsInit;
		bool HasSpec;
		bool BaseValue;

		ValueIntPrint IntPrint;
		std::int16_t Precision;
		std::int16_t FloatPrecision;

		std::size_t Size;

		ShiftPrint ShiftPrint;
		ShiftType ShiftType;
		std::int16_t ShiftValue;

		std::uint8_t SpecIdx;
		std::array<FormatSpecifier, 10> Spec;

	public:
		bool HasChangeColor;

	public:
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

	template<typename T>	struct CopyFormatData				{ static void Copy(FormatData& data, const T& t)			{ } };
	template<>				struct CopyFormatData<FormatData>	{ static void Copy(FormatData& data, const FormatData& t)	{ data = t; } };
}