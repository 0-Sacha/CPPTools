#pragma once

#include "../Detail.h"

namespace CPPTools::Fmt::Detail {

	template <typename CharBuffer>
	class FormatterMemoryBufferOutCopy;

	template <typename CharBuffer>
	class FormatterMemoryBufferOut {
	public:
		using CharBufferType = CharBuffer;

		static constexpr const std::size_t	DEFAULT_BEGIN_SIZE	= 512;
		static constexpr const std::size_t	GROW_UP_BUFFER_SIZE = 2;
		static constexpr const bool			DEBUG_RESIZE		= true;

	private:
		CharBuffer*			m_Buffer;
		CharBuffer*			m_CurrentPos;
		CharBuffer*			m_BufferEnd;	// Point one behind the end char
		std::size_t			m_BufferSize;	// The real allocated size

		bool				m_BufferAutoResize;
		bool				m_FreeOnDestructor;

	public:
		inline CharBuffer*			GetBuffer()									{ return m_Buffer; }
		inline const CharBuffer*	GetBuffer() const							{ return m_Buffer; }
		inline CharBuffer*			GetBufferCurrentPos()						{ return m_CurrentPos; }
		inline const CharBuffer*	GetBufferCurrentPos() const					{ return m_CurrentPos; }
		inline CharBuffer*			GetBufferEnd() 								{ return m_BufferEnd; }
		inline const CharBuffer*	GetBufferEnd() const						{ return m_BufferEnd; }
		inline std::size_t			GetBufferSize() const						{ return m_BufferSize; }
		inline std::size_t			GetBufferCurrentSize() const				{ return m_CurrentPos - m_Buffer; }
		inline void					SetBufferCurrentPos(CharBuffer* const pos)	{ m_CurrentPos = pos; }

		inline bool					BufferIsAutoResize()						{ return m_BufferAutoResize; }

	private:
		friend FormatterMemoryBufferOutCopy<CharBuffer>;
		inline void					DoNotFreeOnDestructor()					{ m_FreeOnDestructor = false; }

	public:
		FormatterMemoryBufferOut(CharBuffer* const buffer, const std::size_t bufferSize)
			: m_Buffer(buffer)
			, m_CurrentPos(buffer)
			, m_BufferEnd(buffer + bufferSize)
			, m_BufferSize(bufferSize)
			, m_BufferAutoResize(false)
			, m_FreeOnDestructor(false)
		{
			PushEndCharOnTheEnd();
		}

		FormatterMemoryBufferOut(const std::size_t beginSize = DEFAULT_BEGIN_SIZE)
			: m_Buffer(new CharBuffer[beginSize])
			, m_CurrentPos(m_Buffer)
			, m_BufferEnd(m_Buffer + beginSize)
			, m_BufferSize(beginSize)
			, m_BufferAutoResize(true)
			, m_FreeOnDestructor(true)
		{
			PushEndCharOnTheEnd();
		}

		~FormatterMemoryBufferOut() {
			if (m_FreeOnDestructor)
				delete[] m_Buffer;
		}

		template <typename ParentBuffer>
		FormatterMemoryBufferOut(ParentBuffer& parentBuffer)
			: m_Buffer(parentBuffer.GetBuffer())
			, m_CurrentPos(parentBuffer.GetBufferCurrentPos())
			, m_BufferEnd(parentBuffer.GetBufferEnd())
			, m_BufferSize(parentBuffer.GetBufferSize())
			, m_BufferAutoResize(parentBuffer.BufferIsAutoResize()) {}

	public:
		template <typename ChildBuffer>
		inline void UpdateFromClidBuffer(ChildBuffer& childBuffer) { SetBufferCurrentPos(childBuffer.GetBufferCurrentPos()); }

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
		inline bool CanMoveForward()									{ if (m_CurrentPos < m_BufferEnd) return true; return CheckResize(); }
		inline bool CanMoveBackward() const								{ return m_CurrentPos > m_Buffer; }
		inline bool CanMoveForward(const std::size_t count) const		{ return m_CurrentPos + count <= m_BufferEnd; }
		inline bool CanMoveBackward(const std::size_t count) const		{ return m_CurrentPos + count >= m_Buffer; }
		inline bool IsNotOutOfBound() const								{ return !CanMoveForward() || !CanMoveBackward(); }

		bool CheckResize() {
			if (!m_BufferAutoResize)	return false;
			return						Resize();
		}

		bool Resize() {
			std::size_t step = m_CurrentPos - m_Buffer;

			const CharBuffer* oldBuffer = m_Buffer;
			const std::size_t oldSize = m_BufferSize;

			m_BufferSize *= GROW_UP_BUFFER_SIZE;
			m_Buffer = new CharBuffer[m_BufferSize];
			if (m_Buffer == nullptr) return false;

			m_CurrentPos = m_Buffer + step;
			m_BufferEnd = m_Buffer + m_BufferSize;

			std::memcpy(m_Buffer, oldBuffer, oldSize);

			delete[] oldBuffer;

			if constexpr (DEBUG_RESIZE)
				std::cout << "Resize from " << oldSize << " to " << m_BufferSize << std::endl;

			return true;
		}


		// Buffer base commands
		inline void Forward()											{ if (CanMoveForward()) ++m_CurrentPos; }
		inline void ForwardNoCheck()									{ ++m_CurrentPos; }
		inline void Backward()											{ if (CanMoveBackward()) --m_CurrentPos; }
		template<typename Int> inline void Forward(const Int size)		{ m_CurrentPos += size; if (!CanMoveForward()) m_CurrentPos = m_BufferEnd; }
		template<typename Int> inline void Backward(const Int size)		{ m_CurrentPos -= size; if (!CanMoveBackward()) m_CurrentPos = m_Buffer; }

		inline CharBuffer Get() const									{ return *m_CurrentPos; }
		inline CharBuffer GetAndForward()								{ return CanMoveForward() ? *m_CurrentPos++ : '\0'; }
		inline CharBuffer GetAndForwardNoCheck()						{ return *m_CurrentPos++; }
		inline CharBuffer GetAndBackward()								{ return CanMoveBackward() ? *m_CurrentPos-- : '\0'; }
		inline CharBuffer GetNext() const								{ return CanMoveForward() ? *(m_CurrentPos + 1) : '\0'; }

		inline void Set(const CharBuffer c)								{ *m_CurrentPos = c; }
		inline void PushBack(const CharBuffer c)						{ if (CanMoveForward()) *m_CurrentPos++ = c; }
		inline void PushReverse(const CharBuffer c)						{ if (CanMoveBackward()) *m_CurrentPos-- = c; }
		inline void PushBackNoCheck(const CharBuffer c)					{ *m_CurrentPos++ = c; }
		inline void PushReverseNoCheck(const CharBuffer c)				{ *m_CurrentPos-- = c; }

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


	template <typename CharBuffer>
	class FormatterMemoryBufferOutCopy {
	public:
		FormatterMemoryBufferOutCopy(FormatterMemoryBufferOut<CharBuffer>& bufferOut)
			: m_Buffer(bufferOut.GetBuffer())
			, m_Size(bufferOut.GetBufferCurrentSize())
			, m_FreeOnDestructor(true)
		{
			bufferOut.DoNotFreeOnDestructor();
		}

		FormatterMemoryBufferOutCopy(FormatterMemoryBufferOutCopy<CharBuffer>& bufferOutCopy)
			: m_Buffer(bufferOutCopy.GetBuffer())
			, m_Size(bufferOutCopy.GetSize())
			, m_FreeOnDestructor(true)
		{
			bufferOutCopy.DoNotFreeOnDestructor();
		}

		~FormatterMemoryBufferOutCopy() {
			if (m_FreeOnDestructor)
				delete[] m_Buffer;
		}

		operator std::basic_string_view<CharBuffer>() {
			return std::basic_string_view<CharBuffer>(m_Buffer, m_Size);
		}


	public:

		inline const CharBuffer*	GetBuffer() const		{ return m_Buffer; }
		inline std::size_t			GetSize() const			{ return m_Size; }

		inline void DoNotFreeOnDestructor() {
			m_FreeOnDestructor = false;
		}

	private:
		const CharBuffer* const m_Buffer;
		const std::size_t		m_Size;

		bool					m_FreeOnDestructor;
	};
}

