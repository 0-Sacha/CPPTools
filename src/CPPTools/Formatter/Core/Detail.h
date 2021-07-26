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
} // CPPTools::Fmt::Detail

namespace CPPTools::Fmt {

	using FormatIdx = int;
	static const FormatIdx FormatIdxNotFound = -1;

	struct FormatSpecifier {

		FormatSpecifier()
			: Name(nullptr, 0)
			, ValueAsText(nullptr, 0)
			, ValueAsNumber(0)
			, ValueIsText(true) {}

		FormatSpecifier(const std::string_view name, const std::string_view value)
			: Name(name)
			, ValueAsText(value)
			, ValueAsNumber(0)
			, ValueIsText(true) {}

		FormatSpecifier(const std::string_view name, const std::size_t value)
			: Name(name)
			, ValueAsText(nullptr, 0)
			, ValueAsNumber(value)
			, ValueIsText(false) {}

		std::string_view Name;
		std::string_view	ValueAsText;
		std::intmax_t		ValueAsNumber;
		bool ValueIsText;
	};

	// TODO : delete SetMaxSize (FomatData must not handle this) ...

	// Only with char
	struct FormatData {
	public:
		explicit FormatData()
			: IsInit(false)
			, HasSpec(false)

			, BaseValue(false)

			, IntPrint(::CPPTools::Fmt::Detail::ValueIntPrint::Int)
			, Precision(0)
			, FloatPrecision(3)

			, ShiftPrint(::CPPTools::Fmt::Detail::ShiftPrint::Space)
			, ShiftType(::CPPTools::Fmt::Detail::ShiftType::Nothing)
			, ShiftValue(0)

			, SpecifierCount(0)

			, HasChangeColor(false)
		{}

		explicit FormatData(const FormatData& other)
			: IsInit(other.IsInit)
			, HasSpec(other.HasSpec)

			, BaseValue(other.BaseValue)

			, IntPrint(other.IntPrint)
			, Precision(other.Precision)
			, FloatPrecision(other.FloatPrecision)

			, ShiftPrint(other.ShiftPrint)
			, ShiftType(other.ShiftType)
			, ShiftValue(other.ShiftValue)

			, SpecifierCount(other.SpecifierCount)
			, Specifier(other.Specifier)

			, HasChangeColor(false)
		{}

		explicit FormatData(bool hasSpec

			, bool baseValue = false

			, Detail::ValueIntPrint intPrint = Detail::ValueIntPrint::Int
			, FormatDataType precision = 0
			, FormatDataType floatPrecision = 3

			, Detail::ShiftPrint shiftPrint = Detail::ShiftPrint::Space
			, Detail::ShiftType shiftType = Detail::ShiftType::Nothing
			, FormatDataType shiftValue = 0)

			: IsInit(true)
			, HasSpec(hasSpec)

			, BaseValue(baseValue)

			, IntPrint(intPrint)
			, Precision(precision)
			, FloatPrecision(floatPrecision)

			, ShiftPrint(shiftPrint)
			, ShiftType(shiftType)
			, ShiftValue(shiftValue)

			, SpecifierCount(0)

			, HasChangeColor(false)
		{}


		FormatData& operator=(const FormatData& other) {
			std::memcpy(this, &other, sizeof(FormatData));
			HasChangeColor = false;
			return *this;
		}

	public:
		inline void Clone(const FormatData& other) 	{ std::memcpy(this, &other, sizeof(FormatData)); }

	public:
		bool IsInit;
		bool HasSpec;

		bool BaseValue;						// = 

		Detail::ValueIntPrint IntPrint; 	// B - X - O - D
		FormatDataType Precision;			// B - X - O - D
		FormatDataType FloatPrecision;		// .

		Detail::ShiftPrint ShiftPrint;		// < - > - ^
		Detail::ShiftType ShiftType; 		// 0
		FormatDataType ShiftValue;			// < - > - ^

		std::uint8_t SpecifierCount;
		std::array<FormatSpecifier, 10> Specifier;

	public:
		bool HasChangeColor;

	public:
		static inline constexpr std::int8_t NotFound() { return (std::numeric_limits<std::int8_t>::max)(); }

		std::string_view GetValueAsTextOfSpecifierOr(const std::string_view str, const std::string_view defaultValue = "") const {
			for (auto i = 0; i < SpecifierCount; ++i)
				if (Specifier[i].ValueIsText == true && Specifier[i].Name == str)
					return Specifier[i].ValueAsText;
			return defaultValue;
		}

		std::size_t GetValueAsNumberOfSpecifierOr(const std::string_view str, const std::size_t defaultValue = 0) const {
			for (std::uint8_t i = 0; i < SpecifierCount; ++i)
				if (Specifier[i].ValueIsText == false && Specifier[i].Name == str)
					return Specifier[i].ValueAsNumber;
			return defaultValue;
		}

		void AddSpecifier(const FormatSpecifier specifier)									{ if (SpecifierCount < Specifier.size()) Specifier[SpecifierCount++] = specifier; }
		void AddSpecifier(const std::string_view name, const std::string_view value)		{ AddSpecifier(FormatSpecifier(name, value)); }
		void AddSpecifier(const std::string_view name, const std::size_t value)				{ AddSpecifier(FormatSpecifier(name, value)); }

	};
} // CPPTools::Fmt

namespace CPPTools::Fmt::Detail {
	template<typename T>	struct CopyFormatData				{ static void Copy(FormatData& data, const T& t) { } };
	template<>				struct CopyFormatData<FormatData>	{ static void Copy(FormatData& data, const FormatData& t) { data = t; } };

	template <typename CharJoin>
	struct FormatSpecifierJoinSpliter {
		std::basic_string_view<CharJoin> 	Str1;
		std::basic_string_view<CharJoin> 	Str2;
		
		bool HasBeenSplited;

		FormatSpecifierJoinSpliter(const std::basic_string_view<CharJoin> str) : HasBeenSplited(false) {
			if(IsSplitNeeded(str)) {
				HasBeenSplited = true;
				std::size_t indexOfNewLine = std::distance(str.cbegin(), std::find(str.cbegin(), str.cend(), '\n'));
				Str1 = std::basic_string_view<CharJoin>(str.data(), indexOfNewLine);
				Str2 = std::basic_string_view<CharJoin>(str.data() + indexOfNewLine + 1, str.size() - indexOfNewLine - 1);
			} else {
				Str1 = str;
				Str2 = std::basic_string_view<CharJoin>(nullptr, 0);
			}
		}

		template <typename FormatContext>
		void Write(FormatContext& context, const std::size_t stride = std::numeric_limits<std::size_t>::max()) const {
			context.BufferWriteStringView(Str1);
			if(HasBeenSplited) {
				context.BufferPushBack('\n');
				stride == std::numeric_limits<std::size_t>::max() ? context.BufferAddSpaces(context.GetStride()) : context.BufferAddSpaces(stride);
			}
			context.BufferWriteStringView(Str2);
		}

		inline static bool IsSplitNeeded(const std::basic_string_view<CharJoin> str) 	{ return std::find(str.cbegin(), str.cend(), '\n') != str.cend(); } 
	};

} // CPPTools::Fmt::Detail

