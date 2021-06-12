#pragma once

#include "../Formater.h"
#include <string>

namespace CPPTools::Fmt {
	
	template<>
		struct FormatType<std::string> {
		static void Write(const std::string& t, Formater& formater) {
			FormatType<char*>::Write(t.c_str(), formater);
		}
	};

}