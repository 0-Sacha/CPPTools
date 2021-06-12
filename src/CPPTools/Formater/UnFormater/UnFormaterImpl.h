#pragma once

#include "UnFormater.h"
#include "../Formater/FormaterImpl.h"

#include "../BaseFormat/BaseFormat.h"
#include "../BaseFormat/ColorFormat.h"
#include "../BaseFormat/Chrono.h"
#include "../BaseFormat/String.h"

#include "Handlers/FormatBufferReader.h"
#include "Handlers/FormatReader.h"

namespace CPPTools::Fmt {

	template<>
	struct FormatType<UnFormatError>
	{
		static void Write(const UnFormatError t, Formater& formater) {
			if (t)
				formater.LittleFormat("FormatType error idx : {} -- Buffer error idx : {}", t.FormatPosError, t.BufferPosError);
			else
				formater.LittleFormat("No-Error");
		}
	};

	template<typename T, typename ...Args>
	void UnFormater::FormatReadRec(uint8_t idx, T& t, Args&& ...args) {
		if (idx == 0)
			FormatType<GetBaseType<T>>::Read(t, *this);
		else
			FormatReadRec(idx - 1, args...);
	}


	template<typename T, typename ...Args>
	void UnFormater::FormatReadRecNamedArgs(const char* const name, NamedArgs<T>& t, Args&& ...args) {
		if (t.IsRightName(name))
			FormatType<T>::Read(t.t, *this);
		else
			FormatReadRecNamedArgs(name, args...);
	}

	template<typename T, typename ...Args>
	void UnFormater::FormatReadRecNamedArgs(const char* const name, T& t, Args&& ...args) {
		FormatReadRecNamedArgs(name, args...);
	}


	template<typename T, typename ...Args>
	void UnFormater::ParameterDataRec(uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)
			CopyFormatData<T>::Copy(m_FormatData, t);
		else
			ParameterDataRec(idx - 1, args...);
	}

	template<typename ...Args>
	void UnFormater::ParameterData(Args&& ...args) {
		if (FormatIsEqual(':')) {
			m_FormatData.HasSpec = true;
			while (!IsEndOfParameter()) {
				FormatNext();
				FormatIgnoreSpace();

				if (FormatIsEqNext('{')) {
					uint8_t dataIdx = 0;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					ParameterDataRec(dataIdx, args...);
					FormatNext();
				}
				else if (FormatIsEqNext('C')) {
					m_FormatData.HasChangeColor = true;
				}
				else if (FormatIsEqNext('>')) {
					m_FormatData.ShiftType = ShiftType::Right;
					FormatReadUInt(m_FormatData.ShiftValue);
				}
				else if (FormatIsEqNext('<')) {
					m_FormatData.ShiftType = ShiftType::Left;
					FormatReadUInt(m_FormatData.ShiftValue);
				}
				else if (FormatIsEqNext('^')) {
					m_FormatData.ShiftType = ShiftType::Center;
					FormatReadUInt(m_FormatData.ShiftValue);
				}
				else if (FormatIsEqNext('b')) {
					m_FormatData.IntPrint = ValueIntPrint::Bin;
					FormatReadUInt(m_FormatData.Precision);
				}
				else if (FormatIsEqNext('x')) {
					m_FormatData.IntPrint = ValueIntPrint::Hex;
					FormatReadUInt(m_FormatData.Precision);
				}
				else if (FormatIsEqNext('o')) {
					m_FormatData.IntPrint = ValueIntPrint::Oct;
					FormatReadUInt(m_FormatData.Precision);
				}
				else if (FormatIsEqNext('d')) {
					m_FormatData.IntPrint = ValueIntPrint::Int;
					FormatReadUInt(m_FormatData.Precision);
				}
				else if (FormatIsEqNext('.')) {
					FormatReadUInt(m_FormatData.FloatPrecision);
				}
				else if (FormatIsEqNext('s')) {
					FormatReadUInt(m_FormatData.Size);
				}
				else if (FormatIsEqNext('=')) {
					m_FormatData.BaseValue = true;
				}
				else {
					const char c = FormatGetAndNext();
					int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				}

				FormatGoToNext(',');
			}
		}
	}


	template<typename ...Args>
	void UnFormater::ParameterType(Args&& ...args) {
		if (FormatIsEqNext('C'))
			GetColorValue();
		else if (FormatIsEqNext('T'))
			GetTimerPrinted();
		else if (FormatIsEqNext('D'))
			GetDatePrinted();
		else if (FormatIsEqNext('I'))
			IgnoreParameter();
		else {
			uint8_t valueIdx;
			FormatData data;
			data.Clone(m_FormatData);
			m_FormatData = FormatData();
			
			const char* name = nullptr;

			if (!FormatReadUInt(valueIdx)) {
				if (FormatIsLowerCase()) {
					name = m_SubFormat;
					FormatGoToNext(':');
				}
				else
					valueIdx = m_ValuesIdx++;
			}

			if (!m_FormatData.IsInit)
				ParameterData(args...);

			if (name == nullptr)
				FormatReadRec(valueIdx, args...);
			else
				FormatReadRecNamedArgs(name, args...);

			m_FormatData.Clone(data);
		}
	}


	template<typename ...Args>
	bool UnFormater::UnFormat(Args&& ...args) {

		bool error = false;

		while (!IsEndOfFormat() && !error) {

			if (CheckUntilNextParameter()) {
				if (FormatIsEqNext('{')) {
					ParameterType(args...);
					GoOutOfParameter();
				}
			}
			else if (!CheckForEscape())
				error = true;
		}

		return error;
	}

	template<size_t SIZE, typename ...Args>
	bool UnFormater::LittleUnFormat(const char (&format)[SIZE], Args&& ...args) {
		const char* const mainFormater = m_SubFormat;
		const size_t mainFormaterSize = m_FormatSize;
		uint8_t mainIdx = m_ValuesIdx;

		m_FormatSize = SIZE;
		m_SubFormat = format;
		m_ValuesIdx = 0;

		bool error = UnFormat(args...);

		m_ValuesIdx = mainIdx;
		m_SubFormat = mainFormater;
		m_FormatSize = mainFormaterSize;
		return error;
	}

	template<typename ...Args>
	bool LittleUnFormat(const std::string& fromat, Args&& ...args) {
		const char* const mainFormater = m_SubFormat;
		const size_t mainFormaterSize = m_FormatSize;
		uint8_t mainIdx = m_ValuesIdx;

		m_FormatSize = formater.GetBufferSize();
		m_SubFormat = format;
		m_ValuesIdx = 0;

		bool error = UnFormat(args...);

		m_ValuesIdx = mainIdx;
		m_SubFormat = mainFormater;
		m_FormatSize = mainFormaterSize;
		return error;
	}

	template<typename ...Args>
	UnFormatError UnFormater::MainUnFormat(Args&& ...args) {
		UnFormatError error;
		if (UnFormat(args...))
			error = UnFormatError(GetFormatSize(), GetBufferSize());
		else if (!IsEndOfBuffer())
			error = UnFormatError(GetFormatSize(), GetBufferSize());
		return error;
	}









	template<typename ...Args>
	UnFormatError UnFormatChar(const char* const buffer, const char* const format, Args&& ...args) {
		Formater::UnFormater formater(format, buffer);
		return formater.MainUnFormat(args...);
	}
}