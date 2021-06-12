#pragma once

#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt {
	template<typename T>
	struct FormatArgs {

		FormatArgs(const uint8_t idx, const T& t)
			: m_Value(t), m_Idx(idx) {}

	public:
		inline bool IsRightIdx(uint8_t idx) const {
			return idx == m_Idx;
		}

	public:
		inline T& GetValue() { return m_Value; }
		inline const T& GetValue() const { return m_Value; }

	protected:
		const T& m_Value;
		const uint8_t m_Idx;
	};

	template<typename T>
	struct FormatType<FormatArgs<T>>
	{
		inline static void Write(const FormatArgs<T>& t, Formater& formater) {
			FormatType<GetBaseType<T>>::Write(t.GetValue(), formater);
		}

		inline static bool Read(const FormatArgs<T>& t, UnFormater& formater) {
			return false;
		}
	};
}

#define FORMAT_IDX(idx, value) CPPTools::Fmt::FormatArgs(idx, value)
