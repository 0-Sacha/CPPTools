#pragma once

#include "TestCore/TestCore.h"
#include "Test/Test.h"

#include "Test/CopyMoveCheck.h"



//***********************************************//
//**************** ALIAS and CMD ****************//
//***********************************************//

#define CPPTOOLS_TEST_GET_ALIAS
#define CPPTOOLS_TEST_GET_CMD

#ifdef CPPTOOLS_TEST_GET_ALIAS

#define T_ASSERT(x)				CPPTOOLS_TEST_ASSERT(x);
#define	T_TEST(x)				CPPTOOLS_TEST_TEST(x);
#define	T_EQUAL(x, y)			CPPTOOLS_TEST_ISEQUAL(x, y) ;
#define	T_NEQUAL(x, y)			CPPTOOLS_TEST_ISNOTEQUAL(x, y);
#define	T_FN(fn, res)			CPPTOOLS_TEST_FN(fn, res);
#define	T_FN_F(fn, res, ...)	CPPTOOLS_TEST_FN_F(fn, res, __VA_ARGS__);

#define K_ASSERT(x)				CPPTOOLS_TESTK_ASSERT(x);
#define	K_TEST(x)				CPPTOOLS_TESTK_TEST(x);
#define	K_EQUAL(x, y)			CPPTOOLS_TESTK_ISEQUAL(x, y) ;
#define	K_NEQUAL(x, y)			CPPTOOLS_TESTK_ISNOTEQUAL(x, y);
#define	K_FN(fn, res)			CPPTOOLS_TESTK_FN(fn, res);
#define	K_FN_F(fn, res, ...)	CPPTOOLS_TESTK_FN_F(fn, res, __VA_ARGS__);

#endif

#ifdef CPPTOOLS_TEST_GET_ALIAS

#define MSG_TRACE(...)		CPPTOOLS_TEST_TRACE(__VA_ARGS__);
#define	MSG_INFO(...)		CPPTOOLS_TEST_INFO(__VA_ARGS__);
#define	MSG_WARN(...)		CPPTOOLS_TEST_WARN(__VA_ARGS__);
#define	MSG_ERROR(...)		CPPTOOLS_TEST_ERROR(__VA_ARGS__);
#define	MSG_FATAL(...)		CPPTOOLS_TEST_FATAL(__VA_ARGS__);

#define	MSG_OK(...)			CPPTOOLS_TEST_OK(__VA_ARGS__);
#define	MSG_FAIL(...)		CPPTOOLS_TEST_FAIL(__VA_ARGS__);

#define	MSG_PRINT(...)		CPPTOOLS_TEST_BASIC(__VA_ARGS__);

#endif


#ifdef CPPTOOLS_TEST_GET_CMD

#define BEGIN(x)	void x(CPPTools::Test::TestMemoryCollisionAvoid testMemory_CollissionAvoider) {\
					std::string_view name_CollissionAvoider(#x);\
					CPPTOOLS_TEST_TRACE("--------------------------------------");\
					CPPTOOLS_TEST_TRACE("Begin of {} in file : {} at line : {}", #x, __FILE__, __LINE__);

#define END()		CPPTOOLS_TEST_TRACE("End of {} : {}", name_CollissionAvoider, testMemory_CollissionAvoider);\
					CPPTOOLS_TEST_TRACE("--------------------------------------");\
					}

#define EXEC(x)		x(testMemory_CollissionAvoider);

#define ADD_VALID()		testMemory_CollissionAvoider.AddValid()
#define ADD_FAIL()		testMemory_CollissionAvoider.AddFail()

#define TEST_LAUNCH()	CPPTOOLS_TEST_SIMPE_MSG()\
						MTests(CPPTools::Test::TestMemoryCollisionAvoid());

#endif