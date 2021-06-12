#pragma once

#include "../Core/Core.h"

#define CPPTOOLS_FORMAT_DECLARED

namespace CPPTools::Fmt {

	class Formater;
	class UnFormater;

	template<typename T>
	struct FormatType {
		static void Write(const T& i, Formater& formater) {
			static_assert(false, __FUNCSIG__);
		}
		static bool Read(T& i, UnFormater& formater) {
			static_assert(false, __FUNCSIG__);
			return false;
		}
	};

}

#define CPPTOOLS_AUTO_FORMAT(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<>\
													struct FormatType<Type> {\
														static void Write(const Type& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
														static bool Read(Type& value, UnFormater& formater) {\
															return formater.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}



#define CPPTOOLS_AUTO_FORMAT_TEMPLATED(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<typename Template>\
													struct FormatType<Type<Template>> {\
														static void Write(const Type<Template>& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
														static bool Read(Type<Template>& value, UnFormater& formater) {\
															return formater.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}



#define CPPTOOLS_AUTO_FORMAT_TEMPLATED_SPECIFIER(Specifier, Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<>\
													struct FormatType<Type<Specifier>> {\
														static void Write(const Type<Specifier>& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
														static bool Read(Type<Specifier>& value, UnFormater& formater) {\
															return formater.LittleUnFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}





#define CPPTOOLS_AUTO_FORMAT_FO(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<>\
													struct FormatType<Type> {\
														static void Write(const Type& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}



#define CPPTOOLS_AUTO_FORMAT_FO_TEMPLATED(Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<typename Template>\
													struct FormatType<Type<Template>> {\
														static void Write(const Type<Template>& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}



#define CPPTOOLS_AUTO_FORMAT__FO_TEMPLATED_SPECIFIER(Specifier, Type, fmt, ...)	namespace CPPTools::Fmt {\
													template<>\
													struct FormatType<Type<Specifier>> {\
														static void Write(const Type<Specifier>& value, Formater& formater) {\
															formater.LittleFormat(fmt, __VA_ARGS__);\
														}\
													};\
												}