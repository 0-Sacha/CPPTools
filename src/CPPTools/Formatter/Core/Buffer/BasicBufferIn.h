#pragma once

#include "../Detail.h"

namespace CPPTools::Fmt::Detail {

	template <typename CharBuffer>
	class BasicFormatterMemoryBufferIn {

	private:
		const CharBuffer*	m_Buffer;
		const CharBuffer*	m_CurrentPos;
		const CharBuffer*	m_BufferEnd;			// Point to the end char of the format
		std::size_t			m_BufferSize;			// Do not count the end char

	public:
		inline const CharBuffer*	GetBuffer() const									{ return m_Buffer; }
		inline const CharBuffer*	GetBufferCurrentPos() const							{ return m_CurrentPos; }
		inline const CharBuffer*	GetBufferEnd() const								{ return m_BufferEnd; }
		inline std::size_t			GetBufferSize() const								{ return m_BufferSize; }
		inline std::size_t			GetBufferCurrentSize() const						{ return m_CurrentPos - m_Buffer; }
		inline void					SetBufferCurrentPos(const CharBuffer* const pos)	{ m_CurrentPos = pos; }
		
	public:
		BasicFormatterMemoryBufferIn(const std::basic_string_view<CharBuffer> bufferIn)
			: m_Buffer(bufferIn.data())
			, m_CurrentPos(bufferIn.data())
			, m_BufferEnd(bufferIn.data() + bufferIn.size())
			, m_BufferSize(bufferIn.size()) {}

	public:
		// Format
		inline bool CanMoveForward() const							{ return m_CurrentPos < m_BufferEnd; }
		inline bool CanMoveBackward() const							{ return m_CurrentPos > m_Buffer; }
		inline bool CanMoveForward(const std::size_t count) const	{ return m_CurrentPos + count <= m_BufferEnd; }
		inline bool CanMoveBackward(const std::size_t count) const	{ return m_CurrentPos + count >= m_Buffer; }
		inline bool IsNotOutOfBound() const							{ return !CanMoveForward() || !CanMoveBackward(); }
		inline bool End() const										{ return m_CurrentPos >= m_BufferEnd; }

		// Format base commands
		inline void Forward()										{ if (CanMoveForward()) ++m_CurrentPos; }
		inline void ForwardNoCheck()								{ ++m_CurrentPos; }
		inline void Backward()										{ if (CanMoveBackward()) --m_CurrentPos; }
		inline void BackwardNoCheck()								{ --m_CurrentPos; }
		template<typename Int> inline void Forward(const Int size)	{ m_CurrentPos += size; if (!CanMoveForward()) m_CurrentPos = m_BufferEnd; }
		template<typename Int> inline void Backward(const Int size) { m_CurrentPos -= size; if (!CanMoveBackward()) m_CurrentPos = m_Buffer; }

		inline CharBuffer Get() const								{ return *m_CurrentPos; }
		inline CharBuffer GetAndForward()							{ return CanMoveForward() ? *m_CurrentPos++ : '\0'; }
		inline CharBuffer GetAndForwardNoCheck()					{ return *m_CurrentPos++; }
		inline CharBuffer GetAndBackward()							{ return CanMoveBackward() ? *m_CurrentPos-- : '\0'; }
		inline CharBuffer GetAndBackwardNoCheck()					{ return *m_CurrentPos--; }
		inline CharBuffer GetNext() const							{ return CanMoveForward() ? *(m_CurrentPos + 1) : '\0'; }
		inline CharBuffer GetNextNoCheck() const					{ return *(m_CurrentPos + 1); }

		// Format check
		inline bool IsEqualTo(const CharBuffer c) const				{ return Get() == c; }
		inline bool IsNotEqualTo(const CharBuffer c) const			{ return Get() != c; }
		inline bool IsEqualForward(const CharBuffer c)				{ if (IsEqualTo(c)) { Forward(); return true; } return false; }
		inline bool IsNotEqualForward(const CharBuffer c)			{ if (IsNotEqualTo(c)) { Forward(); return true; } return false; }
		template<typename ...CharToTest> inline bool IsEqualTo(const CharBuffer c, const CharToTest ...ele) const		{ return IsEqualTo(c) || IsEqualTo(ele...); }
		template<typename ...CharToTest> inline bool IsEqualForward(const CharToTest ...ele)							{ if (IsEqualTo(ele...)) { Forward(); return true; } return false; }
		template<typename ...CharToTest> inline bool IsNotEqualTo(const CharBuffer c, const CharToTest ...ele) const	{ return IsNotEqualTo(c) && IsNotEqualTo(ele...); }
		template<typename ...CharToTest> inline bool IsNotEqualForward(const CharToTest ...ele)							{ if (IsNotEqualTo(ele...)) { Forward(); return true; } return false; }

		// Format Next check
		inline bool NextIsEqualTo(const CharBuffer c) const			{ return GetNext() == c; }
		inline bool NextIsNotEqualTo(const CharBuffer c) const		{ return GetNext() != c; }
		inline bool NextIsEqualForward(const CharBuffer c)			{ Forward(); if (!IsEqualTo(c)) { BackwardNoCheck(); return false; } return true; }
		inline bool NextIsNotEqualForward(const CharBuffer c)		{ Forward(); if (!IsNotEqualTo(c)) { BackwardNoCheck(); return false; } return true; }
		template<typename ...CharToTest> inline bool NextIsEqualForward(const CharToTest ...ele)		{ Forward(); if (IsEqualTo(ele...)) { return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsEqualTo(const CharToTest ...ele) const		{ Forward(); if (IsEqualTo(ele...)) { BackwardNoCheck(); return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsNotEqualForward(const CharToTest ...ele)		{ Forward(); if (IsNotEqualTo(ele...)) { return true; } BackwardNoCheck(); return false; }
		template<typename ...CharToTest> inline bool NextIsNotEqualTo(const CharToTest ...ele) const	{ Forward(); if (IsNotEqualTo(ele...)) { BackwardNoCheck(); return true; } BackwardNoCheck(); return false; }

		template<typename CharToTest> bool NextIsANamedArgs(std::basic_string_view<CharToTest> sv) {
			const CharBuffer* const prevSubFormat = m_CurrentPos;
			if (NextIsSame(sv) && (IsEqualTo(':') || IsEqualTo('}'))) return true;
			m_CurrentPos = prevSubFormat;
			return false;
		}

		template<typename CharToTest> bool NextIsSame(std::basic_string_view<CharToTest> sv) {
			const CharToTest* str = sv.data();
			std::size_t size = sv.size();
			const CharBuffer* prevSubFormat = m_CurrentPos;		bool isSame = true;
			while (isSame && size-- != 0 && CanMoveForward())	isSame = GetAndForwardNoCheck() == *str++;
			if (isSame && size == 0)							isSame = false;
			if (!isSame)										m_CurrentPos = prevSubFormat;
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