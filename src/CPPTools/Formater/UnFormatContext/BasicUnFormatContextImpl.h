#pragma once

#include "BasicUnFormatContext.h"
#include "BasicUnFormatContextClassImpl.h"

#include "../FormatContext/BasicFormatContextImpl.h"


#include "Handlers/FormatBufferReader.h"
#include "Handlers/FormatReader.h"

namespace CPPTools::Fmt {

	template<typename FormatContext>
	struct FormatType<UnFormatContextError, FormatContext>
	{
		static void Write(const UnFormatContextError t, FormatContext& context) {
			if (t)	context.LittleFormat("FormatType error idx : {} -- Buffer error idx : {}", t.FormatPosError, t.BufferPosError);
			else	context.LittleFormat("No-Error");
		}
	};

	/////---------- Default Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::FormatReadRec(std::uint8_t idx, T& t, Args&& ...args) {
		if (idx == 0)	FormatType<GetBaseType<T>>::Read(t, *this);
		else			FormatReadRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- NamedArgs Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::FormatReadRecNamedArgs(const Char* const name, UFCNamedArgs<T, BasicUnFormatContext<Char>>& t, Args&& ...args) {
		if (t.IsRightName(name))	FormatType<UFCFormatArgs<T>>::Read(t, *this);
		else						FormatReadRecNamedArgs(name, std::forward<Args>(args)...);
	}

	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::FormatReadRecNamedArgs(const Char* const name, UFCCStringNamedArgs<T, BasicUnFormatContext<Char>>& t, Args&& ...args) {
		FormatReadRecNamedArgs(name, static_cast<UFCFormatArgs<T>&>(t), std::forward<Args>(args)...);
	}

	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::FormatReadRecNamedArgs(const Char* const name, UFCStringNamedArgs<T, BasicUnFormatContext<Char>>& t, Args&& ...args) {
		FormatReadRecNamedArgs(name, static_cast<UFCFormatArgs<T>&>(t), std::forward<Args>(args)...);
	}

	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::FormatReadRecNamedArgs(const Char* const name, T& t, Args&& ...args) {
		FormatReadRecNamedArgs(name, std::forward<Args>(args)...);
	}

	/////---------- Data Print Rec ----------/////
	template<typename Char>
	template<typename T, typename ...Args>
	inline void BasicUnFormatContext<Char>::ParameterDataRec(std::uint8_t idx, const T& t, Args&& ...args) {
		if (idx == 0)	Detail::CopyFormatData<T>::Copy(m_FormatData, t);
		else			ParameterDataRec(idx - 1, std::forward<Args>(args)...);
	}

	/////---------- Impl ----------/////
	template<typename Char>
	template<typename ...Args>
	void BasicUnFormatContext<Char>::ParameterData(Args&& ...args) {
		if (FormatIsEqualTo(':')) {
			m_FormatData.HasSpec = true;
			while (!FormatIsEndOfParameter()) {
				FormatForward();
				FormatIgnoreSpace();

				if (BufferIsEqualForward('{')) {		// Forward specifier
					std::uint8_t dataIdx;
					if (!FormatReadUInt(dataIdx))
						dataIdx = m_ValuesIdx++;
					ParameterDataRec(dataIdx, std::forward<Args>(args)...);
					FormatForward();
				} else if (BufferIsEqualForward('b')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Bin;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('x')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Hex;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('o')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Oct;	FormatReadUInt(m_FormatData.Precision);
				} else if (BufferIsEqualForward('d')) { m_FormatData.IntPrint = Detail::ValueIntPrint::Int;	FormatReadUInt(m_FormatData.Precision);
				} else if (FormatIsLowerCase()) {	// Custom specifier
					const char c = FormatGetAndForward();
					std::int8_t i = 0;
					FormatReadInt(i);
					m_FormatData.AddSpecifier(c, i);
				} else if (BufferIsEqualForward('C')) {
				} else if (BufferIsEqualForward('>')) { m_FormatData.ShiftType = Detail::ShiftType::Right;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('<')) { m_FormatData.ShiftType = Detail::ShiftType::Left;		FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('^')) { m_FormatData.ShiftType = Detail::ShiftType::Center;	FormatReadUInt(m_FormatData.ShiftValue);
				} else if (BufferIsEqualForward('.')) { FormatReadUInt(m_FormatData.FloatPrecision);
				} else if (BufferIsEqualForward('S')) { FormatReadUInt(m_FormatData.Size);
				} else if (BufferIsEqualForward('B')) { FormatReadUInt(m_FormatData.Begin);
				} else if (BufferIsEqualForward('\n')) { m_FormatData.ContainerPrintStyle = Detail::ContainerPrintStyle::NewLine;
				} else if (BufferIsEqualForward('=')) { m_FormatData.BaseValue = true; }

				FormatParamGoTo(',');
			}
		}
	}

	template<typename Char>
	template<typename ...Args>
	void BasicUnFormatContext<Char>::ParameterType(Args&& ...args) {
		FormatForward();						// '{'

		if (BufferIsEqualForward('C'))			GetColorValue();
		else if (BufferIsEqualForward('T'))		GetTimerPrinted();
		else if (BufferIsEqualForward('D'))		GetDatePrinted();
		else if (BufferIsEqualForward('I'))		IgnoreParameter();
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

			if (!m_FormatData.IsInit)	ParameterData(std::forward<Args>(args)...);

			if (name == nullptr)		FormatReadRec(valueIdx, args...);
			else						FormatReadRecNamedArgs(name, args...);

			m_FormatData.Clone(data);
		}

		FormatGoOutOfParameter();				// '}'
	}

	template<typename Char>
	template<typename ...Args>
	bool BasicUnFormatContext<Char>::UnFormat(Args&& ...args) {

		bool error = false;

		while (!FormatEnd() && !error) {

			if (CheckUntilNextParameter()) {
				if (FormatIsEqualTo('{'))
					if (!CheckForEscape())
						ParameterType(std::forward<Args>(args)...);
			}
			else if (!Check())
				error = true;
		}

		return error;
	}

	template<typename Char>
	template<typename ...Args>
	bool BasicUnFormatContext<Char>::LittleUnFormat(const std::string_view format, Args&& ...args) {
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

		bool error = UnFormat(std::forward<Args>(args)...);

		// Assign old value
		m_Format		= mainFormat;
		m_SubFormat		= mainSubFormat;
		m_FormatEnd		= mainFormatEnd;
		m_FormatSize	= mainFormatSize;
		m_ValuesIdx		= mainIdx;

		return error;
	}

	template<typename Char>
	template<typename ...Args>
	UnFormatContextError BasicUnFormatContext<Char>::MainUnFormat(Args&& ...args) {
		UnFormatContextError error;
		if (UnFormat(args...))		error = UnFormatContextError(GetFormatSize(), GetBufferSize());
		else if (!BufferEnd())		error = UnFormatContextError(GetFormatSize(), GetBufferSize());
		return error;
	}



	/////---------- Function ----------/////
	template<typename Char, typename ...Args>
	UnFormatContextError UnFormat(const std::basic_string_view<Char> buffer, const std::basic_string_view<Char> format, Args&& ...args) {
		Fmt::BasicUnFormatContext context(format, buffer);
		return context.MainUnFormat(std::forward<Args>(args)...);
	}
}