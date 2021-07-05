#pragma once

#include "../Core/UnitTestCore.h"

// Assert log if false
#define UNITTEST_ASSERT(x) if(!(x)) UNITTEST_FAIL("ASSERT ERROR : {0:C:red}", #x)

// Check if false
#define UNITTEST_TEST(x)	if(x)	UNITTEST_OK("TEST OK      : {:C:green}", #x);\
							else	UNITTEST_FAIL("TEST ERROR   : {:C:red}", #x)

// Check if equal with file printed
#define UNITTEST_ISEQUAL(x, y) if(x == y)	UNITTEST_OK("TEST OK      : {C:green}{} == {}", #x, #y);\
							    else		UNITTEST_FAIL("TEST ERROR   : {C:red}{} == {}", #x, #y)

// Check if not equal with file printed
#define UNITTEST_ISNOTEQUAL(x, y)	if(x != y)	UNITTEST_OK("TEST OK      : {C:green}{} != {}", #x, #y);\
									else		UNITTEST_FAIL("TEST ERROR   : {C:red}{} != {}", #x, #y)


// Assert log if false file printed
#define UNITTEST_ASSERT_FILE(x) if(!(x)) UNITTEST_FAIL("ASSERT ERROR : {0:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if true with file printed
#define UNITTEST_TEST_FILE(x)	if(x)	UNITTEST_OK("TEST OK      : {:C:green} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
								else	UNITTEST_FAIL("TEST ERROR   : {:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if equal with file printed
#define UNITTEST_ISEQUAL_FILE(x, y) if(x == y)	UNITTEST_OK("TEST OK      : {C:green}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
									else		UNITTEST_FAIL("TEST ERROR   : {C:red}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if not equal with file printed
#define UNITTEST_ISNOTEQUAL_FILE(x, y)	if(x != y)	UNITTEST_OK("TEST OK      : {C:green}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
										else		UNITTEST_FAIL("TEST ERROR   : {C:red}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))






// Check if false at compile time
#define UNITTEST_STATIC_ASSERT(x) static_assert(x, "ASSERT ERROR : " #x)








