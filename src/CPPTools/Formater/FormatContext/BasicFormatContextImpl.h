#pragma once

#include "BasicFormatContext.h"
#include "BasicFormatContextClassImpl.h"

#include "Handlers/FormatReader.h"
#include "Handlers/FormatWriter.h"


namespace CPPTools::Fmt {

	/////---------- Default Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::FormatPrintRec(std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, *this);
		else			FormatPrintRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::FormatPrintRecNamedArgs(const Char* const name, const FCNamedArgs<T, Char>& t, Args&& ...args) {
		if (t.IsRightName(name))	FormatType<FCNamedArgs<T>, BasicFormatContext<Char>>::Write(t, *this);
		else						FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::FormatPrintRecNamedArgs(const Char* const name, const FCCStringNamedArgs<T, Char>& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, static_cast<const FCNamedArgs<T>&>(t), std::forward<Args>(args)...);
	}
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::FormatPrintRecNamedArgs(const Char* const name, const FCStringNamedArgs<T, Char>& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, static_cast<const FCNamedArgs<T>&>(t), std::forward<Args>(args)...);
	}
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::FormatPrintRecNamedArgs(const Char* const name, const T& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicFormatContext<Char>::ParameterDataRec(std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			ParameterDataRec(idx - 1, std::forward<Args>(args)...);
	}


	/////---------- Impl ----------/////
	template<typename Char>
	template<typename ...Args>
	void BasicFormatContext<Char>::ParameterData(Args&& ...args) {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (FormatIsEqualForward('{')) {		// Forward specifier
					std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					ParameterDataRec(dataIdx, std::forward<Args>(args)...);
					FormatForward();
				} else if (FormatIsEqualForward('b')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('x')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('o')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqualForward('d')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char c = FormatGetAndForward();
					std::int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				} else if (FormatIsEqualForward('C'))		{ m_FormatData.HasChangeColor = true; m_ColorMem = Detail::AnsiColorMem(); ColorValuePrint();
				} else if (FormatIsEqualForward('>'))		{ m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('<'))		{ m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('^'))		{ m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqualForward('.'))		{ FormatReadUInt(m_FormatData.FloatPrecision);
				} else if (FormatIsEqualForward('S'))		{ FormatReadUInt(m_FormatData.Size);
				} else if (FormatIsEqualForward('B'))		{ FormatReadUInt(m_FormatData.Begin);
				} else if (FormatIsEqualForward('\n'))	{ m_FormatData.ContainerPrintStyle = Detail::ContainerPrintStyle::NewLine;
				} else if (FormatIsEqualForward('=')) { m_FormatData.BaseValue = true; }

				FormatParamGoTo(',');
			}
		}
	}

	template<typename Char>
	template<typename ...Args>
	void BasicFormatContext<Char>::ParameterType(Args&& ...args) {
		FormatForward();				// {

		if (FormatIsEqualForward('C'))			ColorValuePrint();
		else if (FormatIsEqualForward('T'))		TimerValuePrint();
		else if (FormatIsEqualForward('D'))		DateValuePrint();
		else {
			std::uint8_t valueIdx;

			FormatData data;
			data.Clone(m_FormatData);
			m_FormatData = FormatData();

			Detail::AnsiColorMem colorMem(m_ColorMem);
			const Char* name = nullptr;

			if (!FormatReadUInt(valueIdx)) {
				if (FormatIsLowerCase())		{ name = m_SubFormat; FormatParamGoTo(':'); }
				else							valueIdx = m_ValuesIdx++;
			}

			if (!m_FormatData.IsInit)			ParameterData(std::forward<Args>(args)...);

			if (name == nullptr)				FormatPrintRec(valueIdx, std::forward<Args>(args)...);
			else								FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);


			if (m_FormatData.HasChangeColor)	{ m_ColorMem = colorMem; ReloadColor(); }
			m_FormatData.Clone(data);
		}

		FormatGoOutOfParameter();	// }
	}

	template<typename Char>
	template<typename ...Args>
	void BasicFormatContext<Char>::Format(Args&& ...args) {
		while (!FormatEnd()) {

			WriteUntilNextParameter();

			if (FormatIsEqualTo('{'))
				if (!CheckForEscape())
					ParameterType(std::forward<Args>(args)...);
		}

	}

	template<typename Char>
	template<typename ...Args>
	void BasicFormatContext<Char>::LittleFormat(const std::basic_string_view<Char> format, Args&& ...args) {
		// Copy
		const Char* const mainFormat		= m_Format;
		const Char* const mainSubFormat		= m_SubFormat;
		const Char* const mainFormatEnd		= m_FormatEnd;
		const std::size_t mainFormatSize	= m_FormatSize;
		std::uint8_t mainIdx = m_ValuesIdx;

		// Assign new value
		m_Format		= format.data();
		m_SubFormat		= format.data();
		m_FormatEnd		= format.data() + format.size();
		m_FormatSize	= format.size();
		m_ValuesIdx		= 0;

		Format(std::forward<Args>(args)...);

		// Assign old value
		m_Format		= mainFormat;
		m_SubFormat		= mainSubFormat;
		m_FormatEnd		= mainFormatEnd;
		m_FormatSize	= mainFormatSize;
		m_ValuesIdx		= mainIdx;
	}

	template<typename Char>
	template<typename ...Args>
	void BasicFormatContext<Char>::MainFormat(Args&& ...args) {
		Format(std::forward<Args>(args)...);
		CheckEndStr();
	}


	/////---------- Impl with as Format ----------//////

	template<typename Char = char, size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(Char (&buffer)[BUFFER_SIZE], const std::basic_string_view<Char> format, Args&& ...args) {
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}

	template<typename Char = char, typename ...Args>
	void FormatInChar(Char* const buffer, const std::size_t bufferSize, const std::basic_string_view<Char> format, Args&& ...args) {
		BasicFormatContext<Char> formater(format, buffer, bufferSize);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	void CFilePrint(FILE* stream, const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	void CFilePrintLn(FILE* stream, const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	void FilePrint(std::basic_ostream<Char>& stream, const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	void FilePrintLn(std::basic_ostream<Char>& stream, const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');
		
		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	void FormatInString(std::basic_string<Char>& str, const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 1024, typename ...Args>
	inline std::basic_string<Char> FormatString(const std::basic_string_view<Char> format, Args&& ...args) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

	/////---------- NO-FORMAT Impl ----------//////

	template<typename Char = char, size_t BUFFER_SIZE, typename T>
	void FormatInChar(Char (&buffer)[BUFFER_SIZE], T&& t) {
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<typename Char = char, typename T>
	void FormatInChar(Char* const buffer, const std::size_t bufferSize, T&& t) {
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, bufferSize);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	void CFilePrint(FILE* stream, T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	void CFilePrintLn(FILE* stream, T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	void FilePrint(std::basic_ostream<Char>& stream, T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	void FilePrintLn(std::basic_ostream<Char>& stream, T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushBack('\n');

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	void FormatInString(std::basic_string<Char>& str, T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<typename Char = char, size_t BUFFER_SIZE = 256, typename T>
	inline std::basic_string<Char> FormatString(T&& t) {
		Char buffer[BUFFER_SIZE];
		BasicFormatContext<Char> formater(std::basic_string_view<Char>(), buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>, BasicFormatContext<Char>>::Write(t, formater);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

}


