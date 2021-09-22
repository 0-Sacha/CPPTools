#pragma once

#include "BasicBufferIn.h"

namespace CPPTools::Fmt::Detail {
	template <typename CharFormat>
	class FormatterMemoryFormat : public BasicFormatterMemoryBufferIn<CharFormat> {

	private:
		using Base = BasicFormatterMemoryBufferIn<CharFormat>;

	public:
		using CharFormatType = CharFormat;

	public:
		FormatterMemoryFormat(const std::basic_string_view<CharFormat> format)
			: BasicFormatterMemoryBufferIn<CharFormat>(format) {}

	public:
		template<typename T> bool ReadInt(T& t);
		template<typename T> bool ReadUInt(T& t);

	public:
		// Format commands in parameter (add check to '}' to avoid skip the end of the format specifier)
		template<typename ...CharToTest> inline void ParamGoTo(const CharToTest ...ele)			{ BasicFormatterMemoryBufferIn<CharFormat>::GoTo(ele..., '}'); }
		template<typename ...CharToTest> inline void ParamGoToForward(const CharToTest ...ele)	{ BasicFormatterMemoryBufferIn<CharFormat>::GoToForward(ele..., '}'); }

		inline bool IsEndOfParameter()		{ return BasicFormatterMemoryBufferIn<CharFormat>::IsEqualTo('}'); }
		inline void GoToEndOfParameter()	{ while (BasicFormatterMemoryBufferIn<CharFormat>::IsNotEqualTo('}') && BasicFormatterMemoryBufferIn<CharFormat>::CanMoveForward()) BasicFormatterMemoryBufferIn<CharFormat>::ForwardNoCheck(); }
		inline void GoOutOfParameter()		{ while (BasicFormatterMemoryBufferIn<CharFormat>::IsNotEqualTo('}') && BasicFormatterMemoryBufferIn<CharFormat>::CanMoveForward()) BasicFormatterMemoryBufferIn<CharFormat>::ForwardNoCheck(); BasicFormatterMemoryBufferIn<CharFormat>::Forward(); }

	};
}