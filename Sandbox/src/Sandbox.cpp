#include "CPPTools/CPPTools.h"

int main()
{
	CPPTOOLS_FORMATTER_TIME_BEGIN();

	CPPTools::LogSystem::GetCoreInstance().LogInfo("Hello World !");

	CPPTools::LogSystem::GetCoreInstance().LogInfo("int: '{}'", 42);

	CPPTools::LogSystem::GetCoreInstance().LogInfo("int shift: '{:>5}'", 42);
	CPPTools::LogSystem::GetCoreInstance().LogInfo("int shift: '{:<5}'", 42);
}