#pragma once

#include "BasicUnFormatContextHelperFile.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/String.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer, ContextArgs&& ...args)
		: m_Buffer(buffer.data())
		, m_SubBuffer(buffer.data())
		, m_BufferEnd(buffer.data() + buffer.size())
		, m_BufferSize(buffer.size())
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ContextArgsSize(sizeof...(ContextArgs))
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename OldCharFormat, typename ...OldContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, BasicUnFormatContext<OldCharFormat, CharBuffer, OldContextArgs...>& oldContext, ContextArgs&& ...args)
		: m_Buffer(oldContext.GetBuffer())
		, m_SubBuffer(oldContext.GetSubBuffer())
		, m_BufferEnd(oldContext.GetBufferEnd())
		, m_BufferSize(oldContext.GetBufferSize())
		, m_Format(format.data())
		, m_SubFormat(format.data())
		, m_FormatEnd(format.data() + format.size())
		, m_FormatSize(format.size())
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ContextArgsSize(sizeof...(ContextArgs))
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(oldContext.GetFormatData());
	}


	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetColorValue() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetTimerPrinted() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::GetDatePrinted() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::IgnoreParameter() {
		if(BufferIsEqualForward(':')) {

		}
		else
			BufferIgnoreSpace();
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharToTest>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatNextIsANamedArgs(std::basic_string_view<CharToTest> sv) {
		const CharFormat* const prevSubFormat = m_SubFormat;
		if (FormatNextIsSame(sv) && FormatIsEqualTo(':') || FormatIsEqualTo('}'))
			return true;
		m_SubFormat = prevSubFormat;
		return false;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharToTest>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::FormatNextIsSame(std::basic_string_view<CharToTest> sv) {
		const CharToTest* str = sv.data();
		std::size_t size = sv.size();
		const CharFormat* prevSubFormat = m_SubFormat;				bool isSame = true;
		while (isSame && size-- != 0 && FormatCanMoveForward())		isSame = FormatGetAndForwardNoCheck() == *str++;
		if (isSame && size == 0)									isSame = false;
		if (!isSame)												m_SubFormat = prevSubFormat;
		return isSame;
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename CharToTest>
	bool BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BufferNextIsSame(std::basic_string_view<CharToTest> sv) {
		const CharToTest* str = sv.data();
		std::size_t size = sv.size();
		const CharBuffer* prevSubBuffer = m_SubBuffer;				bool isSame = true;
		while (isSame && size-- != 0 && BufferCanMoveForward())		isSame = BufferGetAndForwardNoCheck() == *str++;
		if (isSame && size == 0)									isSame = false;
		if (!isSame)												m_SubBuffer = prevSubBuffer;
		return isSame;
	}
}