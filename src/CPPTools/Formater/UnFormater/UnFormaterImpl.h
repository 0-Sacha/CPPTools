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
			if (t)	formater.LittleFormat("FormatType error idx : {} -- Buffer error idx : {}", t.FormatPosError, t.BufferPosError);
			else	formater.LittleFormat("No-Error");
		}
	};

	template<typename T, typename ...Args>
	void UnFormater::FormatReadRec( std::uint8_t idx, T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>>::Read(t, *this);
		else			FormatReadRec(idx - 1, args...);
	}


	template<typename T, typename ...Args>
	void UnFormater::FormatReadRecNamedArgs(const char* const name, NamedArgs<T>& t, Args&& ...args) {
		if (t.IsRightName(name))	FormatType<T>::Read(t.t, *this);
		else						FormatReadRecNamedArgs(name, args...);
	}

	template<typename T, typename ...Args>
	void UnFormater::FormatReadRecNamedArgs(const char* const name, T& t, Args&& ...args) {
		FormatReadRecNamedArgs(name, args...);
	}


	template<typename T, typename ...Args>
	void UnFormater::ParameterDataRec( std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			ParameterDataRec(idx - 1, args...);
	}

	template<typename ...Args>
	void UnFormater::ParameterData(Args&& ...args) {
		if (FormatIsEqual(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatNext();
				FormatParamIgnoreSpace();

				if (FormatIsEqNext('{')) {		// Forward specifier
					std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					ParameterDataRec(dataIdx, std::forward<Args>(args)...);
					FormatNext();
				} else if (FormatIsEqNext('b')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('x')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('o')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsEqNext('d')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char c = FormatGetAndNext();
					std::int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				} else if (FormatIsEqNext('C')) {
				} else if (FormatIsEqNext('>')) { m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('<')) { m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('^')) { m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (FormatIsEqNext('.')) { FormatReadUInt(m_FormatData.FloatPrecision);
				} else if (FormatIsEqNext('S')) { FormatReadUInt(m_FormatData.Size);
				} else if (FormatIsEqNext('B')) { FormatReadUInt(m_FormatData.Begin);
				} else if (FormatIsEqNext('\n')) { m_FormatData.ContainerPrintStyle = Detail::ContainerPrintStyle::NewLine;
				} else if (FormatIsEqNext('=')) { m_FormatData.BaseValue = true; }

				FormatParamGoTo(',');
			}
		}
	}


	template<typename ...Args>
	void UnFormater::ParameterType(Args&& ...args) {
		if (FormatIsEqNext('C'))			GetColorValue();
		else if (FormatIsEqNext('T'))		GetTimerPrinted();
		else if (FormatIsEqNext('D'))		GetDatePrinted();
		else if (FormatIsEqNext('I'))		IgnoreParameter();
		else {
			std::uint8_t valueIdx;
			FormatData data;
			data.Clone(m_FormatData);
			m_FormatData = FormatData();
			
			const char* name = nullptr;

			if (!FormatReadUInt(valueIdx)) {
				if (FormatIsLowerCase())	{ name = m_SubFormat; FormatParamGoTo(':'); }
				else						valueIdx = m_ValuesIdx++;
			}

			if (!m_FormatData.IsInit)	ParameterData(args...);

			if (name == nullptr)		FormatReadRec(valueIdx, args...);
			else						FormatReadRecNamedArgs(name, args...);

			m_FormatData.Clone(data);
		}
	}


	template<typename ...Args>
	bool UnFormater::UnFormat(Args&& ...args) {

		bool error = false;

		while (!FormatIsEndChar() && !error) {

			if (CheckUntilNextParameter()) {
				if (FormatIsEqNext('{')) {
					ParameterType(args...);
					FormatGoOutOfParameter();
				}
			}
			else if (!CheckForEscape())
				error = true;
		}

		return error;
	}


	template<typename ...Args>
	bool UnFormater::LittleUnFormat(const std::string_view format, Args&& ...args) {
		const char* const mainFormater = m_SubFormat;
		const std::size_t mainFormaterSize = m_FormatSize;
		std::uint8_t mainIdx = m_ValuesIdx;

		m_FormatSize = format.size();
		m_SubFormat = format.data();
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
		if (UnFormat(args...))		error = UnFormatError(GetFormatSize(), GetBufferSize());
		else if (!IsEndOfBuffer())	error = UnFormatError(GetFormatSize(), GetBufferSize());
		return error;
	}


	template<typename ...Args>
	UnFormatError UnFormatChar(const char* const buffer, const std::string_view format, Args&& ...args) {
		Fmt::UnFormater formater(format.data(), format.size(), buffer, 0);
		return formater.MainUnFormat(args...);
	}
}