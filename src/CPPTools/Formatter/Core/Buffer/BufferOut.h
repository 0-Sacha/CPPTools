#pragma once

#include "../Detail.h"

namespace CPPTools::Fmt::Detail {

	template <typename CharBuffer>
	class FormatterMemoryBufferOut {
	public:
		using CharBufferType = CharBuffer;

	public:
		CharBuffer*			m_Buffer;
		CharBuffer*			m_SubBuffer;
		CharBuffer*			m_BufferEnd;	// Point one behind the end char
		std::size_t			m_BufferSize;	// The real allocated size

	public:
		inline CharBuffer*			GetBuffer()								{ return m_Buffer; }
		inline const CharBuffer*	GetBuffer() const						{ return m_Buffer; }
		inline CharBuffer*			GetSubBuffer()							{ return m_SubBuffer; }
		inline const CharBuffer*	GetSubBuffer() const					{ return m_SubBuffer; }
		inline CharBuffer*			GetBufferEnd() 							{ return m_BufferEnd; }
		inline const CharBuffer*	GetBufferEnd() const					{ return m_BufferEnd; }
		inline std::size_t			GetSize() const					{ return m_BufferSize; }
		inline std::size_t			GetCurrentSize() const					{ return m_SubBuffer - m_Buffer; }
		inline void					SetSubBuffer(CharBuffer* const pos)		{ m_SubBuffer = pos; }

	public:
		FormatterMemoryBufferOut(CharBuffer* const buffer, const std::size_t bufferSize)
			: m_Buffer(buffer)
			, m_SubBuffer(buffer)
			, m_BufferEnd(buffer + bufferSize)
			, m_BufferSize(bufferSize) {}


		template <typename ParentBuffer>
		FormatterMemoryBufferOut(ParentBuffer& parentBuffer)
			: m_Buffer(parentBuffer.GetBuffer())
			, m_SubBuffer(parentBuffer.GetSubBuffer())
			, m_BufferEnd(parentBuffer.GetBufferEnd())
			, m_BufferSize(parentBuffer.GetSize()) {}

	public:
		template <typename ChildBuffer>
		inline void UpdateFromClidBuffer(ChildBuffer& childBuffer) { SetSubBuffer(childBuffer.GetSubBuffer()); }

	public:
		template<typename T> void BasicWriteInt(T i);
		template<typename T> void BasicWriteInt(T i, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteIntAsBin(T i, FormatDataType def);
		template<typename T> void BasicWriteIntAsBin(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteIntAsHex(T i, FormatDataType def);
		template<typename T> void BasicWriteIntAsHex(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteIntAsOct(T i, FormatDataType def);
		template<typename T> void BasicWriteIntAsOct(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);

		template<typename T> void BasicWriteUInt(T i);
		template<typename T> void BasicWriteUInt(T i, Detail::ShiftType st, FormatDataType shift, Detail::ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteUIntAsBin(T i, FormatDataType def);
		template<typename T> void BasicWriteUIntAsBin(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteUIntAsHex(T i, FormatDataType def);
		template<typename T> void BasicWriteUIntAsHex(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);
		template<typename T> void BasicWriteUIntAsOct(T i, FormatDataType def);
		template<typename T> void BasicWriteUIntAsOct(T i, FormatDataType def, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);


		template<typename T> void BasicWriteFloat(T i, FormatDataType nbDecimal);
		template<typename T> void BasicWriteFloat(T i, FormatDataType nbDecimal, ShiftType st, FormatDataType shift, ShiftPrint sp = ShiftPrint::Space);


	public:
		template<typename T, typename FormatDataCharType> void WriteIntFormatData(T i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> void WriteUIntFormatData(T i, const FormatData<FormatDataCharType>& formatData);
		template<typename T, typename FormatDataCharType> void WriteFloatFormatData(T i, const FormatData<FormatDataCharType>& formatData);

	public:
		// Buffer
		inline bool CanMoveForward() const								{ return m_SubBuffer < m_BufferEnd; }
		inline bool CanMoveBackward() const								{ return m_SubBuffer > m_Buffer; }
		inline bool CanMoveForward(const std::size_t count) const		{ return m_SubBuffer + count <= m_BufferEnd; }
		inline bool CanMoveBackward(const std::size_t count) const		{ return m_SubBuffer + count >= m_Buffer; }
		inline bool IsNotOutOfBound() const								{ return !CanMoveForward() || !CanMoveBackward(); }

		// Buffer base commands
		inline void Forward()											{ if (CanMoveForward()) ++m_SubBuffer; }
		inline void ForwardNoCheck()									{ ++m_SubBuffer; }
		inline void Backward()											{ if (CanMoveBackward()) --m_SubBuffer; }
		template<typename Int> inline void Forward(const Int size)		{ m_SubBuffer += size; if (!CanMoveForward()) m_SubBuffer = m_BufferEnd; }
		template<typename Int> inline void Backward(const Int size)		{ m_SubBuffer -= size; if (!CanMoveBackward()) m_SubBuffer = m_Buffer; }

		inline CharBuffer Get() const									{ return *m_SubBuffer; }
		inline CharBuffer GetAndForward()								{ return CanMoveForward() ? *m_SubBuffer++ : '\0'; }
		inline CharBuffer GetAndForwardNoCheck()						{ return *m_SubBuffer++; }
		inline CharBuffer GetAndBackward()								{ return CanMoveBackward() ? *m_SubBuffer-- : '\0'; }
		inline CharBuffer GetNext() const								{ return CanMoveForward() ? *(m_SubBuffer + 1) : '\0'; }

		inline void Set(const CharBuffer c)								{ *m_SubBuffer = c; }
		inline void PushBack(const CharBuffer c)						{ if (CanMoveForward()) *m_SubBuffer++ = c; }
		inline void PushReverse(const CharBuffer c)						{ if (CanMoveBackward()) *m_SubBuffer-- = c; }
		inline void PushBackNoCheck(const CharBuffer c)					{ *m_SubBuffer++ = c; }
		inline void PushReverseNoCheck(const CharBuffer c)				{ *m_SubBuffer-- = c; }

		// Buffer commands
		inline void PushEndChar()										{ PushBack('\0'); }
		inline void PushEndCharOnTheEnd() 								{ *(m_BufferEnd - 1) = 0; }
		inline void AddSpaces(const std::size_t count)					{ for (std::size_t i = count; i > 0 && CanMoveForward(); --i) PushBackNoCheck(' '); }

		// Buffer commands for C-style string and c++ style string
		template<typename CharStr>	inline void WriteCharPt(const CharStr* str)					  { while (*str != 0) PushBack(*str++); }
		template<typename CharStr>	inline void WriteCharPt(const CharStr* str, std::size_t size) {
			if (CanMoveForward(size))		while (size-- != 0 && *str != 0) PushBackNoCheck(*str++);
			else							while (size-- != 0 && *str != 0) PushBack(*str++);
		}

		template<typename CharStr, std::size_t SIZE>	inline void WriteCharArray(const CharStr(&str)[SIZE])					{ WriteCharPt(str, SIZE); }
		template<typename CharStr>						inline void WriteStringView(const std::basic_string_view<CharStr> str)	{ WriteCharPt(str.data(), str.size()); }
	};

}

