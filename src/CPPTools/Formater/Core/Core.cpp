
#include "CPPToolspch.h"
#include "Core.h"

namespace CPPTools::Fmt {

	FormatData::FormatData()
		: IsInit(false)
		, HasSpec(false)
		, BaseValue(false)
		, IntPrint(ValueIntPrint::Int)
		, Precision(0)
		, FloatPrecision(3)
		, Size(0)
		, ShiftPrint(ShiftPrint::Space)
		, ShiftType(ShiftType::Nothing)
		, ShiftValue(0)
		, SpecIdx(0)
		, HasChangeColor(false)
	{
	}

	FormatData::FormatData(bool baseValue, ValueIntPrint intPrint, std::uint8_t precision, std::uint8_t floatPrecision, ::CPPTools::Fmt::ShiftPrint shiftPrint, ::CPPTools::Fmt::ShiftType shiftType,  std::uint8_t shiftValue)
		: IsInit(true)
		, HasSpec(true)
		, BaseValue(baseValue)
		, IntPrint(intPrint)
		, Precision(precision)
		, FloatPrecision(floatPrecision)
		, Size(0)
		, ShiftPrint(shiftPrint)
		, ShiftType(shiftType)
		, ShiftValue(shiftValue)
		, SpecIdx(0)
		, HasChangeColor(false)
	{
	}

	FormatData::FormatData(const FormatData& other)
		: IsInit(other.IsInit)
		, HasSpec(other.HasSpec)
		, BaseValue(other.BaseValue)
		, IntPrint(other.IntPrint)
		, Precision(other.Precision)
		, FloatPrecision(other.FloatPrecision)
		, Size(other.Size)
		, ShiftPrint(other.ShiftPrint)
		, ShiftType(other.ShiftType)
		, ShiftValue(other.ShiftValue)
		, SpecIdx(other.SpecIdx)
		, HasChangeColor(false)
	{
	};


	void FormatData::Clone(const FormatData& other)
	{
		std::memcpy(this, &other, sizeof(FormatData));
	}


	FormatData& FormatData::operator=(const FormatData& other) {
		std::memcpy(this, &other, sizeof(FormatData));
		HasChangeColor = false;
		return *this;
	}

	const int8_t FormatData::GetValueOf(const char c) const
	{
		for (std::uint8_t i = 0; i < SpecIdx; ++i)
			if (Spec[i].Type == c)
				return Spec[i].Value;
		return NotFound();
	}

}