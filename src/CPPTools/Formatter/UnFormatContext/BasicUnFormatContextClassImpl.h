#pragma once

#include "BasicUnFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/ColorFormat.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/BaseSTDLib.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer, ContextArgs&& ...args)
		: m_BufferIn(buffer)
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ParentCharFormat, typename ...ParentContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, BasicUnFormatContext<ParentCharFormat, CharBuffer, ParentContextArgs...>& parentContext, ContextArgs&& ...args)
		: m_BufferIn(parentContext.BufferIn())
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(parentContext.GetFormatData());
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ChildCharFormat, typename ...ChildContextArgs>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UpdateContextFromChild(BasicFormatContext<ChildCharFormat, CharBuffer, ChildContextArgs...>& childContext) {
		m_BufferIn.UpdateFromClidBuffer(childContext.BufferIn());
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
		if(m_BufferIn.IsEqualForward(':')) {

		}
		else
			m_BufferIn.IgnoreSpace();
	}

}