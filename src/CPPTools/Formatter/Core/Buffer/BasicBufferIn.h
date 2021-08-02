#pragma once

#include "../Detail.h"

namespace CPPTools::Fmt::Detail {

	template <typename CharFormat>
	class BasicFormatterMemoryBufferIn {

	public:
		const CharFormat*	m_Format;
		const CharFormat*	m_SubFormat;
		const CharFormat*	m_FormatEnd;			// Point to the end char of the format
		std::size_t			m_FormatSize;			// Do not count the end char

	public:
		inline const CharFormat*	GetFormat() const							{ return m_Format; }
		inline const CharFormat*	GetSubFormat() const						{ return m_SubFormat; }
		inline const CharFormat*	GetFormatEnd() const						{ return m_FormatEnd; }
		inline std::size_t			GetFormatSize() const						{ return m_FormatSize; }
		inline std::size_t			GetCurrentSize() const						{ return m_SubFormat - m_Format; }
		inline void					SetSubFormat(const CharFormat* const pos)	{ m_SubFormat = pos; }
		
	public:
		BasicFormatterMemoryBufferIn(const std::basic_string_view<CharFormat> format)
			: m_Format(format.data())
			, m_SubFormat(format.data())
			, m_FormatEnd(format.data() + format.size())
			, m_FormatSize(format.size()) {}

	public:
		// Format
		inline bool CanMoveForward() const							{ return m_SubFormat < m_FormatEnd; }
		inline bool CanMoveBackward() const							{ return m_SubFormat > m_Format; }
		inline bool CanMoveForward(const std::size_t count) const	{ return m_SubFormat + count <= m_FormatEnd; }
		inline bool CanMoveBackward(const std::size_t count) const	{ return m_SubFormat + count >= m_Format; }
		inline bool IsNotOutOfBound() const							{ return !CanMoveForward() || !CanMoveBackward(); }
		inline bool End() const										{ return m_SubFormat >= m_FormatEnd; }

		// Format base commands
		inline void Forward()										{ if (CanMoveForward()) ++m_SubFormat; }
		inline void ForwardNoCheck()								{ ++m_SubFormat; }
		inline void Backward()										{ if (CanMoveBackward()) --m_SubFormat; }
		inline void BackwardNoCheck()								{ --m_SubFormat; }
		template<typename Int> inline void Forward(const Int size)	{ m_SubFormat += size; if (!CanMoveForward()) m_SubFormat = m_FormatEnd; }
		template<typename Int> inline void Backward(const Int size) { m_SubFormat -= size; if (!CanMoveBackward()) m_SubFormat = m_Format; }

		inline CharFormat Get() const								{ return *m_SubFormat; }
		inline CharFormat GetAndForward()							{ return CanMoveForward() ? *m_SubFormat++ : '\0'; }
		inline CharFormat GetAndForwardNoCheck()					{ return *m_SubFormat++; }
		inline CharFormat GetAndBackward()							{ return CanMoveBackward() ? *m_SubFormat-- : '\0'; }
		inline CharFormat GetAndBackwardNoCheck()					{ return *m_SubFormat--; }
		inline CharFormat GetNext() const							{ return CanMoveForward() ? *(m_SubFormat + 1) : '\0'; }
		inline CharFormat GetNextNoCheck() const					{ return *(m_SubFormat + 1); }

		// Format check
		inline bool IsEqualTo(const CharFormat c) const				{ return Get() == c; }
		inline bool IsNotEqualTo(const CharFormat c) const			{ return Get() != c; }
		inline bool IsEqualForward(const CharFormat c)				{ if (IsEqualTo(c)) { Forward(); return true; } return false; }
		inline bool IsNotEqualForward(const CharFormat c)			{ if (IsNotEqualTo(c)) { Forward(); return true; } return false; }
		template<typename ...CharToTest> inline bool IsEqualTo(const CharFormat c, const CharToTest ...ele) const		{ return IsEqualTo(c) || IsEqualTo(ele...); }
		template<typename ...CharToTest> inline bool IsEqualForward(const CharToTest ...ele) const						{ if (IsEqualTo(ele...)) { Forward(); return true; } return false; }
		template<typename ...CharToTest> inline bool IsNotEqualTo(const CharFormat c, const CharToTest ...ele) const	{ return IsNotEqualTo(c) && IsNotEqualTo(ele...); }
		template<typename ...CharToTest> inline bool IsNotEqualForward(const CharToTest ...ele) const					{ if (IsNotEqualTo(ele...)) { Forward(); return true; } return false; }

		// Format Next check
		inline bool NextIsEqualTo(const CharFormat c) const			{ return GetNext() == c; }
		inline bool NextIsNotEqualTo(const CharFormat c) const		{ return GetNext() != c; }
		inline bool NextIsEqualForward(const CharFormat c)			{ Forward(); if (!IsEqualTo(c)) { BackwardNoCheck(); return false; } return true; }
		inline bool NextIsNotEqualForward(const CharFormat c)		{ Forward(); if (!IsNotEqualTo(c)) { BackwardNoCheck(); return false; } return true; }
		template<typename ...CharToTest> inline bool NextIsEqualForward(const CharToTest ...ele)		{ Forward(); if (IsEqualTo(ele...)) { return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsEqualTo(const CharToTest ...ele) const		{ Forward(); if (IsEqualTo(ele...)) { BackwardNoCheck(); return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsNotEqualForward(const CharToTest ...ele)		{ Forward(); if (IsNotEqualTo(ele...)) { return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsNotEqualTo(const CharToTest ...ele) const	{ Forward(); if (IsNotEqualTo(ele...)) { BackwardNoCheck(); return true; } BackwardNoCheck(); return false; }

		template<typename CharToTest> bool NextIsANamedArgs(std::basic_string_view<CharToTest> sv) {
			const CharFormat* const prevSubFormat = m_SubFormat;
			if (NextIsSame(sv) && (IsEqualTo(':') || IsEqualTo('}'))) return true;
			m_SubFormat = prevSubFormat;
			return false;
		}

		template<typename CharToTest> bool NextIsSame(std::basic_string_view<CharToTest> sv) {
			const CharToTest* str = sv.data();
			std::size_t size = sv.size();
			const CharFormat* prevSubFormat = m_SubFormat;		bool isSame = true;
			while (isSame && size-- != 0 && CanMoveForward())	isSame = GetAndForwardNoCheck() == *str++;
			if (isSame && size == 0)							isSame = false;
			if (!isSame)										m_SubFormat = prevSubFormat;
			return isSame;
		}

		template<std::size_t SIZE, typename CharToTest>
		inline bool NextIsSame(const CharToTest(&data)[SIZE]) { return NextIsSame(std::basic_string_view<CharToTest>(data)); }

		inline bool IsLowerCase() const	{ return Get() >= 'a' && Get() <= 'z'; }
		inline bool IsUpperCase() const	{ return Get() >= 'A' && Get() <= 'Z'; }
		inline bool IsADigit() const	{ return Get() >= '0' && Get() <= '9'; }

		// Format commands
		inline void IgnoreSpace()															{ while (IsEqualTo(' ') && CanMoveForward()) ForwardNoCheck(); }
		template<typename ...CharToTest> inline void GoTo(const CharToTest ...ele)			{ while (IsNotEqualTo(ele...) && CanMoveForward())	ForwardNoCheck(); }
		template<typename ...CharToTest> inline void GoToForward(const CharToTest ...ele)	{ while (IsNotEqualTo(ele...) && CanMoveForward())	ForwardNoCheck(); Forward(); }
	};
}