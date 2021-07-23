#pragma once

#include "FormatType.h"

// TODO : - Add check for '\n' to get stride
// 		  - Constructor

namespace CPPTools::Fmt {

	class STDEnumerableUtility {
	public:
		static constexpr std::string_view DefaultBegin	= "{ ";
		static constexpr std::string_view DefaultJoin	= ", ";
		static constexpr std::string_view DefaultEnd	= " }";
	};

	template <typename T, typename CharJoin = char, typename CharBegin = char, typename CharEnd = char>
	class STDEnumerable {
	public:

		inline STDEnumerable(const T& value,
			const std::basic_string_view<CharBegin> strBegin = "{ ",
			const std::basic_string_view<CharJoin> strJoin = ", ",
			const std::basic_string_view<CharEnd> strEnd = " }",
			const std::size_t beginIdx = 0,
			const std::size_t size = std::numeric_limits<std::size_t>::max())
			: m_Value(value)
			, m_StrBegin(strBegin)
			, m_JoinSplited(strJoin)
			, m_StrEnd(strEnd)
			, m_BeginIdx(beginIdx)
			, m_Size(size == std::numeric_limits<std::size_t>::max() ? value.size() - beginIdx : size)
		{
		}

		inline const T& GetValue() const { return m_Value; }

		inline const Detail::FormatSpecifierJoinSpliter<CharJoin>&	GetJoinSplited() const	{ return m_JoinSplited; }

		inline std::basic_string_view<CharBegin>	GetStrBegin() const	{ return m_StrBegin; }
		inline std::basic_string_view<CharEnd>		GetStrEnd() const	{ return m_StrEnd; }

		inline std::size_t GetBeginIdx() const	{ return m_BeginIdx; }
		inline std::size_t GetSize() const		{ return m_Size; }

	private:
		const T& m_Value;

		Detail::FormatSpecifierJoinSpliter<CharJoin>	m_JoinSplited;

		std::basic_string_view<CharBegin> m_StrBegin;
		std::basic_string_view<CharEnd>	m_StrEnd;

		std::size_t m_BeginIdx;
		std::size_t m_Size;
	};

	template <typename T, typename CharBegin, typename CharJoin, typename CharEnd, typename FormatContext>
	struct FormatType<STDEnumerable<T, CharBegin, CharJoin, CharEnd>, FormatContext> {
		static void Write(const STDEnumerable<T, CharBegin, CharJoin, CharEnd>& enumerable, FormatContext& context) {

			context.BufferWriteStringView(enumerable.GetStrBegin());

			std::size_t stride = context.GetStride();

			bool first = true;
			std::for_each_n(enumerable.GetValue().cbegin() + enumerable.GetBeginIdx(), enumerable.GetSize(), [&](const auto& element) {
				if (first)	first = false;
				else		{ enumerable.GetJoinSplited().Write(context, stride); }
				context.WriteType(element); });

			context.BufferWriteStringView(enumerable.GetStrEnd());
		}
	};


	template<typename T>
	struct ForwardAsSTDEnumerable {};

	template <typename T, typename FormatContext>
	struct FormatType<ForwardAsSTDEnumerable<T>, FormatContext> {
		static void Write(const T& container, FormatContext& context) {
			STDEnumerable<T> enumerable(container,
				context.GetFormatData().GetValueAsTextOfSpecifierOr("begin", STDEnumerableUtility::DefaultBegin),
				context.GetFormatData().GetValueAsTextOfSpecifierOr("join", STDEnumerableUtility::DefaultJoin),
				context.GetFormatData().GetValueAsTextOfSpecifierOr("end", STDEnumerableUtility::DefaultEnd),
				context.GetFormatData().GetValueAsNumberOfSpecifierOr("begin", 0),
				context.GetFormatData().GetValueAsNumberOfSpecifierOr("size", std::numeric_limits<std::size_t>::max()));

			context.WriteType(enumerable);
		}
	};

}