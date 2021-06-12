#pragma once

#include "FormatInclude.h"

namespace CPPTools::Fmt {


	template<>
	struct FormatType<FormatSpecifier> {
		static void Write(const FormatSpecifier& t, Formater& formater) {
			formater.BufferPushBack('{');
			formater.BufferPushBack(' ');
			formater.BufferPushBack(t.Type);
			formater.BufferPushBack(';');
			formater.BufferPushBack(' ');
			formater.BufferParseInt(t.Value);
			formater.BufferPushBack(' ');
			formater.BufferPushBack('}');
		}
	};

	template<>
	struct FormatType<FormatData> {
		static void Write(const FormatData& t, Formater& formater) {
			formater.LittleFormat("{:C:red}", "Missing '{' or '}' because currently the format data is used as a parameter");
		}
		static bool Read(FormatData& t, UnFormater& formater) {
			return false;
		}
	};




	template<>
	struct FormatType<bool> {
		static void Write(const bool t, Formater& formater) {
			const FormatData& data = formater.GetFormatData();

			if (data.BaseValue) {
				if (t == true)	formater.BufferParseCharPt("True");
				else			formater.BufferParseCharPt("False");
			} else {
				if (t == true)	formater.BufferPushBack('1');
				else			formater.BufferPushBack('0');
			}
		}
		static bool Read(bool& t, UnFormater& formater) {
			const FormatData& data = formater.GetFormatData();

			if (data.BaseValue) {
				switch (formater.BufferGet())
				{
				case 'T':
				case 't':
					if (formater.BufferNextIsSame("True"))	t = true;
					return true;
				case 'F':
				case 'f':
					if (formater.BufferNextIsSame("False"))	t = false;
					return true;
				}
				return false;
			}
			else {
				if (formater.BufferIsEqNext('0'))		t = false;
				else if (formater.BufferIsEqNext('1'))	t = true;
					else
						return false;
			}
			return true;
		}
	};


	template<>
	struct FormatType<char> {
		static void Write(const char t, Formater& formater) {
			formater.BufferPushBack(t);
		}
		static bool Read(char& t, UnFormater& formater) {
			t = formater.BufferGetAndNext();
			return true;
		}
	};

#if 0
	template<>
	struct FormatType<unsigned char> {
		static void Write(const unsigned char t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(unsigned char& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<short> {
		static void Write(const short t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(short& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<unsigned short> {
		static void Write(const unsigned short t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(unsigned short& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<int> {
		static void Write(const long t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(long& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<unsigned int> {
		static void Write(const unsigned int t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(unsigned int& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};
#endif

	template<>
	struct FormatType<long> {
		static void Write(const long t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(long& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<unsigned long> {
		static void Write(const unsigned long t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(unsigned long& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<float> {
		static void Write(const float t, Formater& formater) {
			formater.BufferParseFloat((double)t);
		}
		static bool Read(float& t, UnFormater& formater) {
			return formater.BufferUnParseFloat(t);
		}
	};

	template<>
	struct FormatType<double> {
		static void Write(const double t, Formater& formater) {
			formater.BufferParseFloat(t);
		}
		static bool Read(double& t, UnFormater& formater) {
			return formater.BufferUnParseFloat(t);
		}
	};

	template<>
	struct FormatType<long double> {
		static void Write(const long double t, Formater& formater) {
			formater.BufferParseFloat(t);
		}
		static bool Read(long double& t, UnFormater& formater) {
			return formater.BufferUnParseFloat(t);
		}
	};



	//------------------ STD Type ------------------//

	template<>
	struct FormatType<std::int8_t> {
		static void Write(const std::int8_t t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(std::int8_t& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<std::uint8_t> {
		static void Write(const std::uint8_t t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(std::uint8_t& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<std::int16_t> {
		static void Write(const std::int16_t t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(std::int16_t& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<std::uint16_t> {
		static void Write(const std::uint16_t t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(std::uint16_t& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<std::int32_t> {
		static void Write(const std::int32_t t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(std::int32_t& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<std::uint32_t> {
		static void Write(const std::uint32_t t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(std::uint32_t& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};

	template<>
	struct FormatType<std::int64_t> {
		static void Write(const std::int64_t t, Formater& formater) {
			formater.BufferParseInt(t);
		}
		static bool Read(std::int64_t& t, UnFormater& formater) {
			return formater.BufferUnParseInt(t);
		}
	};

	template<>
	struct FormatType<std::uint64_t> {
		static void Write(const std::uint64_t t, Formater& formater) {
			formater.BufferParseUInt(t);
		}
		static bool Read(std::uint64_t& t, UnFormater& formater) {
			return formater.BufferUnParseUInt(t);
		}
	};


	
	//------------------ Pointer/Array of Type ------------------//

	template<typename T>
	struct FormatType<T*>
	{
		static void Write(const T* const t, Formater& formater) {
			if(t == nullptr)	formater.LittleFormat("{}", (void*)t);
			else				formater.LittleFormat("{} -> {:{}}", (void*)t, *t, formater.ForwardFormatData());
		}
		static bool Read(T*& t, UnFormater& formater) {
			return false;
		}
	};

	template<>
	struct FormatType<void*>
	{
		static void Write(const void* const t, Formater& formater) {
			if (t == nullptr)	formater.LittleFormat("nullptr");
			else				formater.LittleFormat("0x{:x}", (std::size_t)t);
		}
		static bool Read(void*& t, UnFormater& formater) {
			return false;
		}
	};

	template<std::size_t SIZE, typename T>
	struct FormatType<T[SIZE]>
	{
		static void Write(T const (t)[SIZE], Formater& formater) {
			const FormatData& data = formater.GetFormatData();

			std::int8_t begin = data.GetValueOf('a');
			if (begin == FormatData::NotFound())	begin = 0;

			std::size_t size = SIZE - begin;
			if (data.Size != 0)		size = data.Size;

			const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

			formater.BufferPushBack('[');

			for (t += begin; size > 1; --size) {
				FormatType<T>::Write(*t++, formater);
				formater.BufferParseCharPt(nextElement);
			}
			if(size == 1)	FormatType<T>::Write(*t, formater);

			formater.BufferPushBack(']');
		}
		static bool Read(T (t)[SIZE], UnFormater& formater) {
			const FormatData& data = formater.GetFormatData();

			if (formater.BufferIsEqNext('[')) {

				std::int8_t begin = data.GetValueOf('a');
				if (begin == FormatData::NotFound()) begin = 0;

				std::size_t size = SIZE - begin;
				if (data.Size != 0)		size = data.Size;

				const char* nextElement = data.GetValueOf('n') == FormatData::NotFound() ? ", " : "\n";

				for (t += begin; size > 1; --size) {
					if (!FormatType<T>::Read(*t++, formater))		return false;
					if (!formater.BufferNextIsSame(nextElement))	return false;
				}

				if (size == 1)						FormatType<T>::Read(*t, formater);
				if (formater.BufferIsEqNext(']'))	return true;
			}
			return false;
		}
	};



	//------------------ Specifier for Pointer/Array of Type ------------------//

	template<>
	struct FormatType<char*> {
		static void Write(const char* const t, Formater& formater) {
			const FormatData& data = formater.GetFormatData();

			std::int8_t begin = data.GetValueOf('a');
			if (begin == FormatData::NotFound())	begin = 0;

			if (data.BaseValue)						formater.BufferPushBack('"');

			if (data.Size != 0)						formater.BufferParseCharPt(t + begin, data.Size);
			else									formater.BufferParseCharPt(t + begin);

			if (data.BaseValue)						formater.BufferPushBack('"');
		}
		static bool Read(char* t, UnFormater& formater) {
			const FormatData& data = formater.GetFormatData();

			if (data.Size != 0) {
				bool res = formater.BufferUnParseCharPt(t, data.Size);
				if (data.GetValueOf('e') != FormatData::NotFound()) *t = 0;
				return res;
			}
			return false;
		}
	};

	template<size_t SIZE>
	struct FormatType<char[SIZE]> {
		static void Write(char const (t)[SIZE], Formater& formater) {
			const FormatData& data = formater.GetFormatData();

			std::int8_t begin = data.GetValueOf('a');
			if (begin == FormatData::NotFound()) begin = 0;

			std::size_t size = SIZE - begin;
			if (data.Size != 0)	size = data.Size;

			if (data.BaseValue)									formater.BufferPushBack('"');

			for (t += begin; size > 0 && *t != '\0'; --size)	formater.BufferPushBack(*t++);

			if (data.BaseValue)									formater.BufferPushBack('"');
		}
		static bool Read(char (t)[SIZE], UnFormater& formater) {
			const FormatData& data = formater.GetFormatData();

			std::int8_t begin = data.GetValueOf('a');
			if (begin == FormatData::NotFound())	begin = 0;

			std::size_t size = SIZE - begin;
			if (data.Size != 0)						size = data.Size;

			std::int8_t putEnd = data.GetValueOf('e');
			if (putEnd != FormatData::NotFound())	--size;

			char* str = t;
			for (; size > 0; --size)				*str++ = formater.BufferGetAndNext();
			if (putEnd != FormatData::NotFound())	*str = 0;
			return *(str - 1) != 0;
		}
	};

}