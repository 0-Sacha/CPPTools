

#include <iostream>
#include <thread>

#include "CPPTools/LogSystem/LogSystem.h"
#include "CPPTools/Profiling/Profiling.h"
#include "CPPTools/Test/Test.h"


int test(int i) {
	return 5 * i;
}

int main() {
	CPPTOOLS_FORMATTER_TIME_BEGIN();
	
	CPPTOOLS_TEST_FNFMT_FILE(test(5), 25, "test");
	CPPTOOLS_TEST_FNFMT_FILE(test(4), 25, "test");
}