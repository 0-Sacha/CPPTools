#pragma once

#include "CPPTools/Core/Core.h"
#include "UnFormatType.h"

namespace CPPTools::Fmt {
	template<typename T, typename UnFormatContext>
	struct UFCFormatArgs {

		UFCFormatArgs(const std::uint8_t idx, T& t)
			: m_Value(t), m_Idx(idx) {}

	public:
		inline bool IsRightIdx(std::uint8_t idx) const { return idx == m_Idx; }

	public:
		inline T& GetValue() { return m_Value; }

	protected:
		T& m_Value;
		const std::uint8_t m_Idx;
	};

	template<typename T, typename UnFormatContext>
	struct UnFormatType<UFCFormatArgs<T, UnFormatContext>, UnFormatContext>
	{
		template<typename Char>
		inline static void Read(UFCFormatArgs<T, UnFormatContext>& t, UnFormatContext& context) {
			return FormatType<GetBaseType<T>>::Write(t.GetValue(), context);
		}
	};
}
