#pragma once

#include "../BasicFormatContext.h"

// According to : https://en.wikipedia.org/wiki/ANSI_escape_code

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<Detail::AnsiTextFront, FormatContext>
	{
		static void Write(const Detail::AnsiTextFront t, FormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicWriteType('\033', '[', t.FrontId, 'm');
		}
	};

}


