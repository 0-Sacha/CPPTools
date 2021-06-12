#pragma once

#include "../Core/UnitTestCore.h"

// Check if false
#define UNIT_TEST_ASSERT(x) if(!(x)) UNIT_TEST_FAIL("ASSERT ERROR : {0:C:red}", #x)


#define UNIT_TEST_TEST(x)	if(x)	UNIT_TEST_OK("TEST OK      : {:C:green}", #x);\
							else	UNIT_TEST_FAIL("TEST ERROR   : {:C:red}", #x)

#define UNIT_TEST_ISEQUAL(x, y) if(x == y)	UNIT_TEST_OK("TEST OK      : {C:green}{} == {}", #x, #y);\
							    else		UNIT_TEST_FAIL("TEST ERROR   : {C:red}{} == {}", #x, #y)

#define UNIT_TEST_ISNOTEQUAL(x, y)	if(x != y)	UNIT_TEST_OK("TEST OK      : {C:green}{} != {}", #x, #y);\
									else		UNIT_TEST_FAIL("TEST ERROR   : {C:red}{} != {}", #x, #y)


// Check if false
#define UNIT_TEST_ASSERT_FILE(x) if(!(x)) UNIT_TEST_FAIL("ASSERT ERROR : {0:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__))


#define UNIT_TEST_TEST_FILE(x)	if(x)	UNIT_TEST_OK("TEST OK      : {:C:green} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__));\
							else		UNIT_TEST_FAIL("TEST ERROR   : {:C:red} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__))

#define UNIT_TEST_ISEQUAL_FILE(x, y) if(x == y)	UNIT_TEST_OK("TEST OK      : {C:green}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__));\
							    else			UNIT_TEST_FAIL("TEST ERROR   : {C:red}{} == {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__))

#define UNIT_TEST_ISNOTEQUAL_FILE(x, y)	if(x != y)	UNIT_TEST_OK("TEST OK      : {C:green}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__));\
									else			UNIT_TEST_FAIL("TEST ERROR   : {C:red}{} != {} -> function : {function:C:+black} ; file : {file:C:+black} ; line : {line:C:+black}", #x, #y, FORMAT_CSTR("function", __FUNCSIG__), FORMAT_CSTR("file", __FILE__), FORMAT_CSTR("line", __LINE__))






// Check if false
#define STATIC_UNIT_TEST_ASSERT(x) static_assert(x, "ASSERT ERROR : " #x)








