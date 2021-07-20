#pragma once

#include "../TestCore/TestCore.h"

// Assert log if false
#define CPPTOOLS_TEST_ASSERT(x) if(!(x)) CPPTOOLS_TEST_FAIL("ASSERT ERROR : {0:C:red}", #x)

// Check if false
#define CPPTOOLS_TEST_TEST(x)	if(x)	CPPTOOLS_TEST_OK("TEST OK      : {:C:green}", #x);\
								else	CPPTOOLS_TEST_FAIL("TEST ERROR   : {:C:red}", #x)

// Check if equal with file printed
#define CPPTOOLS_TEST_ISEQUAL(x, y) if(x == y)	CPPTOOLS_TEST_OK("TEST OK      : {C:green}{} == {}", #x, #y);\
									else		CPPTOOLS_TEST_FAIL("TEST ERROR   : {C:red}{} == {}", #x, #y)

// Check if not equal with file printed
#define CPPTOOLS_TEST_ISNOTEQUAL(x, y)	if(x != y)	CPPTOOLS_TEST_OK("TEST OK      : {C:green}{} != {}", #x, #y);\
										else		CPPTOOLS_TEST_FAIL("TEST ERROR   : {C:red}{} != {}", #x, #y)


// Assert log if false file printed
#define CPPTOOLS_TEST_ASSERT_FILE(x) if(!(x)) CPPTOOLS_TEST_FAIL("ASSERT ERROR : {0:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if true with file printed
#define CPPTOOLS_TEST_TEST_FILE(x)	if(x)	CPPTOOLS_TEST_OK("TEST OK      : {:C:green} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
									else	CPPTOOLS_TEST_FAIL("TEST ERROR   : {:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if equal with file printed
#define CPPTOOLS_TEST_ISEQUAL_FILE(x, y)	if(x == y)	CPPTOOLS_TEST_OK("TEST OK      : {C:green}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
											else		CPPTOOLS_TEST_FAIL("TEST ERROR   : {C:red}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))

// Check if not equal with file printed
#define CPPTOOLS_TEST_ISNOTEQUAL_FILE(x, y)	if(x != y)	CPPTOOLS_TEST_OK("TEST OK      : {C:green}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__));\
											else		CPPTOOLS_TEST_FAIL("TEST ERROR   : {C:red}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_SV("function", __FUNCSIG__), FORMAT_SV("file", __FILE__), FORMAT_SV("line", __LINE__))






// Check if false at compile time
#define CPPTOOLS_TEST_STATIC_ASSERT(x) static_assert(x, "ASSERT ERROR : " #x)








