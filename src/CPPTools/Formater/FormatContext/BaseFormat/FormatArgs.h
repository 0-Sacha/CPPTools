#pragma once

#include "CPPTools/Core/Core.h"
#include "FormatType.h"

namespace CPPTools::Fmt {
	template<typename T, typename FormatContext>
	struct FCFormatArgs {

		FCFormatArgs(const std::uint8_t idx, const T& t)
			: m_Value(t), m_Idx(idx) {}

	public:
		inline bool IsRightIdx(std::uint8_t idx) const { return idx == m_Idx; }

	public:
		inline const T& GetValue() const { return m_Value; }

	protected:
		const T& m_Value;
		const std::uint8_t m_Idx;
	};

	template<typename T, typename FormatContext>
	struct FormatType<FCFormatArgs<T, FormatContext>, FormatContext>
	{
		template<typename Char>
		inline static void Write(const FCFormatArgs<T, FormatContext>& t, FormatContext& context) {
			FormatType<GetBaseType<T>>::Write(t.GetValue(), context);
		}
	};
}
