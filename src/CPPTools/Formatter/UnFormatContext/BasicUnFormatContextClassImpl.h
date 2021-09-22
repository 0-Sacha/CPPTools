#pragma once

#include "BasicUnFormatContext.h"

#include "BaseFormat/BaseFormat.h"
#include "BaseFormat/BaseAnsiTextColor.h"
#include "BaseFormat/BaseAnsiTextStyle.h"
#include "BaseFormat/Chrono.h"
#include "BaseFormat/BaseSTDLib.h"

namespace CPPTools::Fmt {

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, const std::basic_string_view<CharBuffer> buffer, ContextArgs&& ...args)
		: m_BufferIn(buffer)
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ParentCharFormat, typename ...ParentContextArgs>
	BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::BasicUnFormatContext(const std::basic_string_view<CharFormat> format, BasicUnFormatContext<ParentCharFormat, CharBuffer, ParentContextArgs...>& parentContext, ContextArgs&& ...args)
		: m_BufferIn(parentContext.BufferIn())
		, m_FormatStr(format)
		, m_ContextArgs(std::forward<ContextArgs>(args)...)
		, m_NoStride(0)
		, m_ValuesIdx(0)
	{
		m_FormatData.Clone(parentContext.GetFormatData());
	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	template<typename ChildCharFormat, typename ...ChildContextArgs>
	inline void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::UpdateContextFromChild(BasicFormatContext<ChildCharFormat, CharBuffer, ChildContextArgs...>& childContext) {
		m_BufferIn.UpdateFromChildBuffer(childContext.BufferIn());
	}


	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReadAnsiTextColorParameter() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReadAnsiTextStyleParameter() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReadAnsiTextFrontParameter() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReadTimerParameter() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::ReadDateParameter() {

	}

	template<typename CharFormat, typename CharBuffer, typename ...ContextArgs>
	void BasicUnFormatContext<CharFormat, CharBuffer, ContextArgs...>::IgnoreParameter() {
		if(m_BufferIn.IsEqualForward(':')) {

		} else
			m_BufferIn.IgnoreSpace();
	}

}