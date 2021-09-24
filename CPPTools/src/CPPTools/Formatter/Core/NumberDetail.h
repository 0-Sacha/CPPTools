#pragma once

#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt::Detail::ValuesDetail {
	
	template<typename T>
	struct NumberDetail {
		bool IsFloatingNumber	= std::is_floating_point_v<T>;
		bool IsSigned			= std::is_signed_v<T>;
	};

	template<typename T>
	struct FloatDetail {
		using IntType = std::intmax_t;
	};

	template<>
	struct FloatDetail<float> {
		using IntType = std::int32_t;
	};

	template<>
	struct FloatDetail<double> {
		using IntType = std::int64_t;
	};
}