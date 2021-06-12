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
	void Formater::FormatPrintRec( std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>>::Write(t, *this);
		else			FormatPrintRec(idx - 1, std::forward<Args>(args)...);
	}


	/////---------- NamedArgs Print Rec ----------/////
	template<typename T, typename ...Args>
	void Formater::FormatPrintRecNamedArgs(const char* const name, const NamedArgs<T>& t, Args&& ...args) {
		if (t.IsRightName(name))	FormatType<NamedArgs<T>>::Write(t, *this);
		else						FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	void Formater::FormatPrintRecNamedArgs(const char* const name, const T& t, Args&& ...args) {
		FormatPrintRecNamedArgs(name, std::forward<Args>(args)...);
	}


	/////---------- Data Print Rec ----------/////
	template<typename T, typename ...Args>
	void Formater::ParameterDataRec( std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	CopyFormatData<T>::Copy(m_FormatData, t);
		else			ParameterDataRec(idx - 1, std::forward<Args>(args)...);
	}


	/////---------- Impl ----------/////
	template<typename ...Args>
	void Formater::ParameterData(Args&& ...args) {
		if (FormatIsEqual(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatNext();
				FormatParamIgnoreSpace();

				if (FormatIsEqNext('{')) {
					 std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					ParameterDataRec(dataIdx, std::forward<Args>(args)...);
					FormatNext();
				} else if (FormatIsEqNext('C')) {
					m_FormatData.HasChangeColor = true;
					m_ColorMem = AnsiColorMem();
					ColorValuePrint();
				} else if (FormatIsEqNext('>')) {
					m_FormatData.ShiftType = ShiftType::Right;
					FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('<')) {
					m_FormatData.ShiftType = ShiftType::Left;
					FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('^')) {
					m_FormatData.ShiftType = ShiftType::Center;
					FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('b')) {
					m_FormatData.IntPrint = ValueIntPrint::Bin;
					FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('x')) {
					m_FormatData.IntPrint = ValueIntPrint::Hex;
					FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('o')) {
					m_FormatData.IntPrint = ValueIntPrint::Oct;
					FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('d')) {
					m_FormatData.IntPrint = ValueIntPrint::Int;
					FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('.')) { FormatReadUInt(m_FormatData.FloatPrecision);
				} else if (FormatIsEqNext('s')) { FormatReadUInt(m_FormatData.Size);
				} else if (FormatIsEqNext('=')) { m_FormatData.BaseValue = true;
				} else {
					const char c = FormatGetAndNext();
					int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				}

				FormatParamGoTo(',');
			}

		}
	}


	template<typename ...Args>
	void Formater::ParameterType(Args&& ...args) {
		FormatNext();				// {

		if (FormatIsEqNext('C'))		ColorValuePrint();
		else if (FormatIsEqNext('T'))	TimerValuePrint();
		else if (FormatIsEqNext('D'))	DateValuePrint();
		else {
			std::uint8_t valueIdx;

			FormatData data;
			data.Clone(m_FormatData);
			m_FormatData = FormatData();

			AnsiColorMem colorMem(m_ColorMem);
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
		while (!FormatIsEndChar()) {

			WriteUntilNextParameter();

			if (FormatIsEqual('{'))
				if (!CheckForEscape())
					ParameterType(std::forward<Args>(args)...);
		}

	}

	template<typename ...Args>
	void Formater::MainFormat(Args&& ...args) {
		Format(std::forward<Args>(args)...);
		CheckEndStr();
	}

	template<typename ...Args>
	void Formater::LittleFormat(const std::string_view format, Args&& ...args) {
		const char* const mainFormater = m_SubFormat;
		const std::size_t mainFormaterSize = m_FormatSize;
		 std::uint8_t mainIdx = m_ValuesIdx;

		m_FormatSize = format.size();
		m_SubFormat = format.data();
		m_ValuesIdx = 0;

		Format(std::forward<Args>(args)...);

		m_ValuesIdx = mainIdx;
		m_SubFormat = mainFormater;
		m_FormatSize = mainFormaterSize;
	}


	/////---------- Impl with as Format ----------//////

	template<size_t BUFFER_SIZE, typename ...Args>
	void FormatInChar(char(&buffer)[BUFFER_SIZE], const std::string_view format, Args&& ...args) {
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	void CFilePrint(FILE* stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	void CFilePrintLn(FILE* stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	void FilePrint(std::ostream& stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	void FilePrintLn(std::ostream& stream, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushBack('\n');
		
		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	void FormatInString(std::string& str, const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<size_t BUFFER_SIZE = 300,  typename ...Args>
	inline std::string FormatString(const std::string_view format, Args&& ...args) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(format.data(), format.size(), buffer, BUFFER_SIZE);
		formater.MainFormat(std::forward<Args>(args)...);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

	/////---------- NO-FORMAT Impl ----------//////

	template<size_t BUFFER_SIZE, typename T>
	void FormatInChar(char (&buffer)[BUFFER_SIZE], T&& t) {
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	void CFilePrint(FILE* stream, T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	void CFilePrintLn(FILE* stream, T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushBack('\n');

		std::fwrite(formater.GetBuffer(), formater.GetCurrentBufferSize(), 1, stream);
		std::fflush(stream);
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	void FilePrint(std::ostream& stream, T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	void FilePrintLn(std::ostream& stream, T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushBack('\n');

		stream.write(formater.GetBuffer(), formater.GetCurrentBufferSize());
		stream.flush();
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	void FormatInString(std::string& str, T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
		str = formater.GetBuffer();
	}

	template<size_t BUFFER_SIZE = 60, typename T>
	inline std::string FormatString(T&& t) {
		char buffer[BUFFER_SIZE];
		Fmt::Formater formater(nullptr, 0, buffer, BUFFER_SIZE);
		FormatType<GetBaseType<T>>::Write(t, formater);
		formater.BufferPushEndChar();
		return formater.GetBuffer();
	}

}


