#pragma once

#include "CPPTools/Core/Core.h"

namespace CPPTools::Fmt::Detail {

	// According to : https://en.wikipedia.org/wiki/ANSI_escape_code

	struct AnsiTextFront {
	public:
		static inline constexpr std::uint8_t DefaultFront	= 0;
		static inline constexpr std::uint8_t MinFront		= 0;
		static inline constexpr std::uint8_t MaxFront		= 9;

		static inline constexpr std::uint8_t FrontAnsiIdx	= 10;

	public:
		AnsiTextFront(const std::uint8_t frontId = DefaultFront)
			: FrontId((frontId > MaxFront ? DefaultFront : frontId) + FrontAnsiIdx) {}
		
	public:
		std::uint8_t FrontId;
	};

	struct AnsiTextCurrentFront {
	public:
		AnsiTextFront Front;
	};

	struct AnsiTextFrontChange
	{
	public:
		bool HasChangeFront = false;
	};


	const static inline AnsiTextFront			RESET_ANSI_FRONT(AnsiTextFront::DefaultFront);
}

