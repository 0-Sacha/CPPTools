#pragma once

#include "CPPTools/Core/Core.h"

#include "BaseAnsiTextColor.h"
#include "BaseAnsiTextStyle.h"
#include "BaseAnsiTextFront.h"
#include "GetBaseType.h"

#include "NumberDetail.h"

// TODO :
//
//		End writing all writer and reader	
// 
//		REGEX		
//			{I:REGEX}
//			{:regex=REGEX} (string / char array)
//			{}
//
//		PackSave (foreshadowing for constexpr)
//
//


namespace CPPTools::Fmt::Detail {
	using FormatDataType	= std::int16_t;
	static inline constexpr FormatDataType FORMAT_DATA_NOT_SPECIFIED	= (std::numeric_limits<FormatDataType>::max)();
	static inline constexpr FormatDataType SHIFT_NOT_SPECIFIED			= 0;
	static inline constexpr FormatDataType DIGIT_SIZE_NOT_SPECIFIED			= FORMAT_DATA_NOT_SPECIFIED;
	static inline constexpr FormatDataType FLOAT_PRECISION_NOT_SPECIFIED	= 2;
} // namespace CPPTools::Fmt::Detail

//ToRemove
namespace CPPTools::Fmt {
	using FormatDataType = Detail::FormatDataType;
	static inline constexpr FormatDataType FORMAT_DATA_NOT_SPECIFIED = Detail::FORMAT_DATA_NOT_SPECIFIED;
} // namespace CPPTools::Fmt

namespace CPPTools::Fmt::Detail {

	template<typename T> struct ForwardAsInt;
	template<typename T> struct ForwardAsUInt;
	template<typename T> struct ForwardAsFloat;
	template<typename CharType> struct ForwardAsChar {};
	template<typename CharType, std::size_t SIZE> struct ForwardAsCharArray {};
	template<typename CharType> struct ForwardAsCharPt {};

	enum class ValueIntPrint : FormatDataType {
		Int,
		Bin,
		Hex,
		Oct,
		Default = Int
	};

	enum class ShiftType : FormatDataType {
		Nothing,
		Right,
		Left,
		Center,
		Default = Nothing
	};

	enum class ShiftPrint : FormatDataType {
		Space,
		Zeros,
		Default = Space
	};

	enum class PrintStyle : FormatDataType {
		UpperCase,
		LowerCase,
		Nothing,
		Default = UpperCase
	};


} // CPPTools::Fmt::Detail

namespace CPPTools::Fmt {

	// Type for dealing with index
	using FormatIdx = int;
	static const FormatIdx FORMAT_IDX_NOT_FOUND = -1;

	template <typename CharFormat>
	struct FormatSpecifier {

		FormatSpecifier()
			: Name(nullptr, 0)
			, ValueAsText(nullptr, 0)
			, ValueAsNumber(0)
			, ValueIsText(true) {}

		FormatSpecifier(const std::basic_string_view<CharFormat> name, const std::basic_string_view<CharFormat> value)
			: Name(name)
			, ValueAsText(value)
			, ValueAsNumber(0)
			, ValueIsText(true) {}

		FormatSpecifier(const std::basic_string_view<CharFormat> name, const Detail::FormatDataType value)
			: Name(name)
			, ValueAsText(nullptr, 0)
			, ValueAsNumber(value)
			, ValueIsText(false) {}

		static inline constexpr Detail::FormatDataType SpecifierAsNumberNotSpecified()				{ return Detail::FORMAT_DATA_NOT_SPECIFIED; }
		static inline constexpr std::basic_string_view<CharFormat> SpecifierAsTextNotSpecified()	{ return std::basic_string_view<CharFormat>(""); }

		std::basic_string_view<CharFormat>	Name;
		std::basic_string_view<CharFormat>	ValueAsText;
		Detail::FormatDataType				ValueAsNumber;
		bool ValueIsText;
	};

	template <typename CharFormat>
	struct FormatData {
	public:
		explicit FormatData()
			: IsInit(false)
			, HasSpec(false)

			, TrueValue(false)

			, IntPrint(::CPPTools::Fmt::Detail::ValueIntPrint::Default)
			, DigitSize(Detail::DIGIT_SIZE_NOT_SPECIFIED)
			, FloatPrecision(Detail::FLOAT_PRECISION_NOT_SPECIFIED)

			, PrintStyle(Detail::PrintStyle::Default)

			, ShiftPrint(::CPPTools::Fmt::Detail::ShiftPrint::Default)
			, ShiftType(::CPPTools::Fmt::Detail::ShiftType::Default)
			, ShiftValue(Detail::SHIFT_NOT_SPECIFIED)

			, SpecifierCount(0)

			, AnsiTextColorChange()
			, AnsiTextStyleChange()
			, AnsiTextFrontChange()
		{}

		template <typename CharFormatOther>
		explicit FormatData(const FormatData<CharFormatOther>& other)
			: IsInit(other.IsInit)
			, HasSpec(other.HasSpec)

			, TrueValue(other.TrueValue)

			, IntPrint(other.IntPrint)
			, DigitSize(other.DigitSize)
			, FloatPrecision(other.FloatPrecision)

			, PrintStyle(other.PrintStyle)

			, ShiftPrint(other.ShiftPrint)
			, ShiftType(other.ShiftType)
			, ShiftValue(other.ShiftValue)

			, SpecifierCount(other.SpecifierCount)
			, Specifier(other.Specifier)

			, AnsiTextColorChange()
			, AnsiTextStyleChange()
			, AnsiTextFrontChange()
		{}

		explicit FormatData(bool hasSpec

			, bool baseValue = false

			, Detail::ValueIntPrint intPrint		= Detail::ValueIntPrint::Default
			, Detail::FormatDataType digitSize		= Detail::DIGIT_SIZE_NOT_SPECIFIED
			, Detail::FormatDataType floatPrecision = Detail::FLOAT_PRECISION_NOT_SPECIFIED

			, Detail::PrintStyle printStyle			= Detail::PrintStyle::Default 

			, Detail::ShiftPrint shiftPrint		= Detail::ShiftPrint::Default
			, Detail::ShiftType shiftType		= Detail::ShiftType::Default
			, Detail::FormatDataType shiftValue = Detail::SHIFT_NOT_SPECIFIED)

			: IsInit(true)
			, HasSpec(hasSpec)

			, TrueValue(baseValue)

			, IntPrint(intPrint)
			, DigitSize(digitSize)
			, FloatPrecision(floatPrecision)

			, PrintStyle(printStyle)

			, ShiftPrint(shiftPrint)
			, ShiftType(shiftType)
			, ShiftValue(shiftValue)

			, SpecifierCount(0)

			, AnsiTextColorChange()
			, AnsiTextStyleChange()
			, AnsiTextFrontChange()
		{}


		FormatData& operator=(const FormatData& other) {
			std::memcpy(this, &other, sizeof(FormatData));
			AnsiTextColorChange = Detail::AnsiTextColorChange();
			AnsiTextStyleChange = Detail::AnsiTextStyleChange();
			AnsiTextFrontChange = Detail::AnsiTextFrontChange();
			return *this;
		}

	public:
		inline void Clone(const FormatData& other) 	{ std::memcpy(this, &other, sizeof(FormatData)); }

	public:
		bool IsInit;
		bool HasSpec;

		bool TrueValue;							// = 

		Detail::ValueIntPrint IntPrint; 		// B  - X  - O  - D
		Detail::FormatDataType DigitSize;		// B? - X? - O? - D?
		Detail::FormatDataType FloatPrecision;	// .

		Detail::PrintStyle PrintStyle;			// U  - L

		Detail::ShiftPrint ShiftPrint;			// <  - >  - ^
		Detail::FormatDataType ShiftValue;		// <? - >? - ^?
		Detail::ShiftType ShiftType; 			// 0

		std::uint8_t SpecifierCount;
		std::array<FormatSpecifier<CharFormat>, 10> Specifier;

		Detail::AnsiTextColorChange AnsiTextColorChange;
		Detail::AnsiTextStyleChange AnsiTextStyleChange;
		Detail::AnsiTextFrontChange AnsiTextFrontChange;

	public:
		static inline constexpr std::uint8_t NotFound() { return (std::numeric_limits<std::uint8_t>::max)(); }

		std::basic_string_view<CharFormat> GetSpecifierAsText(const std::basic_string_view<CharFormat> str, const std::basic_string_view<CharFormat> defaultValue = FormatSpecifier<CharFormat>::SpecifierAsTextNotSpecified()) const {
			for (auto i = 0; i < SpecifierCount; ++i)
				if (Specifier[i].ValueIsText == true && Specifier[i].Name == str)
					return Specifier[i].ValueAsText;
			return defaultValue;
		}

		Detail::FormatDataType GetSpecifierAsNumber(const std::basic_string_view<CharFormat> str, const Detail::FormatDataType defaultValue = FormatSpecifier<CharFormat>::SpecifierAsNumberNotSpecified()) const {
			for (std::uint8_t i = 0; i < SpecifierCount; ++i)
				if (Specifier[i].ValueIsText == false && Specifier[i].Name == str)
					return Specifier[i].ValueAsNumber;
			return defaultValue;
		}

		void AddSpecifier(const FormatSpecifier<CharFormat> specifier)														{ if (SpecifierCount < Specifier.size()) Specifier[SpecifierCount++] = specifier; }
		void AddSpecifier(const std::basic_string_view<CharFormat> name, const std::basic_string_view<CharFormat> value)	{ AddSpecifier(FormatSpecifier<CharFormat>(name, value)); }
		void AddSpecifier(const std::basic_string_view<CharFormat> name, const Detail::FormatDataType value)				{ AddSpecifier(FormatSpecifier<CharFormat>(name, value)); }

	};
} // CPPTools::Fmt

namespace CPPTools::Fmt::Detail {
	template<typename T, typename CharFormat>	struct CopyFormatData										{ static void Copy(FormatData<CharFormat>& data, const T& t) { } };
	template<typename CharFormat>				struct CopyFormatData<FormatData<CharFormat>, CharFormat>	{ static void Copy(FormatData<CharFormat>& data, const FormatData<CharFormat>& t) { data = t; } };

	template <typename CharJoin>
	struct FormatSpecifierJoinSpliter {
		std::basic_string_view<CharJoin> 	Str1;
		std::basic_string_view<CharJoin> 	Str2;
		
		bool HasBeenSplited;

		FormatSpecifierJoinSpliter(const std::basic_string_view<CharJoin> str) : HasBeenSplited(false) {
			if(IsSplitNeeded(str)) {
				HasBeenSplited = true;
				auto indexOfNewLine = std::distance(str.cbegin(), std::find(str.cbegin(), str.cend(), '\n'));
				Str1 = std::basic_string_view<CharJoin>(str.data(), indexOfNewLine);
				Str2 = std::basic_string_view<CharJoin>(str.data() + indexOfNewLine + 1, str.size() - indexOfNewLine - 1);
			} else {
				Str1 = str;
				Str2 = std::basic_string_view<CharJoin>(nullptr, 0);
			}
		}

		template <typename FormatContext>
		void Write(FormatContext& context, const std::size_t stride = (std::numeric_limits<std::size_t>::max)()) const {
			context.BufferOut().WriteStringView(Str1);
			if(HasBeenSplited) {
				context.BufferOut().PushBack('\n');
				stride == (std::numeric_limits<std::size_t>::max)() ? context.BufferOut().AddSpaces(context.GetStride()) : context.BufferOut().AddSpaces(stride);
			}
			context.BufferOut().WriteStringView(Str2);
		}

		inline static bool IsSplitNeeded(const std::basic_string_view<CharJoin> str) 	{ return std::find(str.cbegin(), str.cend(), '\n') != str.cend(); } 
	};


	template <typename FormatContext>
	struct NoStrideFunction {
		inline NoStrideFunction(FormatContext& context)
			: Context(context)
			, SizeBuffer(Context.StrideGetBufferCurrentSize()) {}

		~NoStrideFunction() {
			Context.AddNoStride(Context.StrideGetBufferCurrentSize() - SizeBuffer);
		}

		FormatContext&	Context;
		std::size_t		SizeBuffer;
	};


	struct AnsiFormatterChange {
	public:
		bool HasMadeChange = false;
	};

} // CPPTools::Fmt::Detail

