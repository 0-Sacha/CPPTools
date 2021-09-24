#pragma once

#include "../BasicUnFormatContextClassImpl.h"

// According to : https://en.wikipedia.org/wiki/ANSI_escape_code

namespace CPPTools::Fmt {

	template<typename UnFormatContext>
	struct UnFormatType<Detail::AnsiTextFront, UnFormatContext>
	{
		static bool Read(const Detail::AnsiTextFront t, UnFormatContext& context) {
			Detail::NoStrideFunction nostride(context);

			context.BasicReadType('\033', '[', t.FrontId, 'm');
		}
	};

}


