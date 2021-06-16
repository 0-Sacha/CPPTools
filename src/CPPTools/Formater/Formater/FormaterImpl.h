#pragma once

#include "Formater.h"
#include "Handlers/FormatWriter.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

namespace CPPTools::Fmt {

	/////---------- Default Print Rec ----------/////
	template<typename T, typename ...Args>
	inline void Formater::FormatPrintRec(std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>>::Write(t, *this);
		else			FormatPrintRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename T, typename ...Args>
	inline void Formater::FormatPrintRecNamedArgs(const char* const name, const NamedArgs<T>& t, Args&& ...args) {
		if (t.IsRightName(name))	FormatType<NamedArgs<T>>::Write(t, *this);
		else						FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	inline void Formater::FormatPrintRecNamedArgs(const char* const name, const CharPtNamedArgs<T>& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
	}
	template<typename T, typename ...Args>
	inline void Formater::FormatPrintRecNamedArgs(const char* const name, const StringNamedArgs<T>& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, static_cast<const NamedArgs<T>&>(t), std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	inline void Formater::FormatPrintRecNamedArgs(const char* const name, const T& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	template<typename T, typename ...Args>
	inline void Formater::ParameterDataRec(std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			ParameterDataRec(idx - 1, std::forward<Args>(args)...);
	}


	/////---------- Impl ----------/////
	template<typename ...Args>
	void Formater::ParameterData(Args&& ...args) {
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


	template<typename ...Args>
	void Formater::ParameterType(Args&& ...args) {
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
			const char* name = nullptr;

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

	template<typename ...Args>
	void Formater::Format(Args&& ...args) {
		while (!FormatEnd()) {

			WriteUntilNextParameter();

			if (FormatIsEqualTo('{'))
				if (!CheckForEscape())
					ParameterType(std::forward<Args>(args)...);
		}

	}

	template<typename ...Args>
	void Formater::LittleFormat(const std::string_view format, Args&& ...args) {
		// Copy
		const char* const mainFormat		= m_Format;
		const char* const mainSubFormat		= m_SubFormat;
		const char* const mainFormatEnd		= m_FormatEnd;
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

	template<typename ...Args>
	void Formater::MainFormat(Args&& ...args) {
		Format(std::forward<Args>(args)...);
		CheckEndStr();
	}


	/////---------- Impl with as Format ----------//////

	template<size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(char(&buffer)[BUFFER_SIZE], const std::string_view format, Args&& ...args) {
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}

	template<typename ...Args>
	void FormatInChar(char* const buffer, const std::size_t bufferSize, const std::string_view format, Args&& ...args) {
		Fmt::Formater formater(format, buffer, bufferSize);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	void CFilePrint(FILE* stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	void CFilePrintLn(FILE* stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	void FilePrint(std::ostream& stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	void FilePrintLn(std::ostream& stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');
		
		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	void FormatInString(std::string& str, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<size_t BUFFER_SIZE = 500,  typename ...Args>
	inline std::string FormatString(const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format, buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

	/////---------- NO-FORMAT Impl ----------//////

	template<size_t BUFFER_SIZE, typename T>
	void FormatInChar(char (&buffer)[BUFFER_SIZE], T&& t, Fmt::FormatData& data = FormatData()) {
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<typename T>
	void FormatInChar(char const buffer, const std::size_t bufferSize, T&& t, Fmt::FormatData& data = FormatData()) {
		Fmt::Formater formater(std::string_view(), buffer, bufferSize);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	void CFilePrint(FILE* stream, T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	void CFilePrintLn(FILE* stream, T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	void FilePrint(std::ostream& stream, T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	void FilePrintLn(std::ostream& stream, T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushBack('\n');

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	void FormatInString(std::string& str, T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<size_t BUFFER_SIZE = 300, typename T>
	inline std::string FormatString(T&& t, Fmt::FormatData& data = FormatData()) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(std::string_view(), buffer, BUFFER_SIZE);
		formater.GetFormatData() = data;
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

}


