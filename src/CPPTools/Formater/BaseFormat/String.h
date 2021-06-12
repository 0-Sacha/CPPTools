#pragma once

#include "../Formater.h"
#include <string>
#include <string_view>
#include <sstream>

namespace CPPTools::Fmt {
	
	template<>
		struct FormatType<std::string> {
		inline static void Write(const std::string& t, Formater& formater) {
			formater.BufferParseCharPt(t.c_str());
		}
	};

	template<>
	struct FormatType<std::string_view> {
		inline static void Write(const std::string_view& t, Formater& formater) {
			formater.BufferParseCharPt(t.data(), t.size());
		}
	};

	template<>
	struct FormatType<std::stringstream> {
		inline static void Write(const std::stringstream& t, Formater& formater) {
			FormatType<std::string>::Write(t.str(), formater);
		}
	};
}