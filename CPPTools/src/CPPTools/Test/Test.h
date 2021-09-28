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

#define TF_ASSERT(x)			CPPTOOLS_TEST_ASSERT_FILE(x);
#define	TF_TEST(x)				CPPTOOLS_TEST_TEST_FILE(x);
#define	TF_EQUAL(x, y)			CPPTOOLS_TEST_ISEQUAL_FILE(x, y) ;
#define	TF_NEQUAL(x, y)			CPPTOOLS_TEST_ISNOTEQUAL_FILE(x, y);
#define	TF_FN(fn, res)			CPPTOOLS_TEST_FN_FILE(fn, res);
#define	TF_FN_F(fn, res, ...)	CPPTOOLS_TEST_FN_F_FILE(fn, res, __VA_ARGS__);

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

#define BEGIN(x)	void x(CPPTools::Test::TestCollisionAvoider) {\
					CPPTOOLS_TEST_TRACE("--------------------------------------");\
					CPPTOOLS_TEST_TRACE("Begin of {}", #x);

#define END(x)		CPPTOOLS_TEST_TRACE("End of {}", #x);\
					CPPTOOLS_TEST_TRACE("--------------------------------------");\
					}

#define EXEC(x)		x(CPPTools::Test::TestCollisionAvoider());

#define TEST_LAUNCH()	CPPTOOLS_TEST_SIMPE_MSG()\
						EXEC(MTests);

#endif