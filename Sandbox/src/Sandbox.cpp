

#include <iostream>
#include <thread>

#include "CPPTools/LogSystem/LogSystem.h"
#include "CPPTools/Profiling/Profiling.h"


int main() {
	CPPTOOLS_FORMATTER_TIME_BEGIN();

	int i = 2, j = 3;

	CPPTools::Fmt::UnFormatContextError error = CPPTools::Fmt::UnFormat("Hello 5 world 8 !", "Hello {I}{} {I}world {I}{}{I}!", i, j);

	CPPTools::LogSystem::GetClientInstance().LogInfo("Res: {i} {j} ; error : {error}", FORMAT(i), FORMAT(j), FORMAT(error));
}