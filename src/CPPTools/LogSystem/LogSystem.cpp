
#include "CPPToolspch.h"

#ifndef CPPTOOLS_BASE_LOGGER_NAME
	#define CPPTOOLS_BASE_LOGGER_NAME "APP"
#endif

#include "LogSystem.h"

namespace CPPTools {

	LogSystem::LogSystem(const std::string& name, const LogSeverity severityMin, std::ostream& stream)
		: m_Name(name), m_SeverityMin(severityMin), m_Stream(stream)
	{
		ResetFormat();
	}

	LogSystem::LogSystem(std::string&& name, const LogSeverity severityMin, std::ostream& stream)
		: m_Name(std::move(name)), m_SeverityMin(severityMin), m_Stream(stream)
	{
		ResetFormat();
	}

	LogSystem::LogSystem()
		: m_Name("Logger"), m_SeverityMin(LogSeverity::Trace), m_Stream(std::cout)
	{
		ResetFormat();
	}

	LogSystem::~LogSystem() = default;


	LogSystem& LogSystem::GetCoreInstance()
	{
		static LogSystem instance(CPPTOOLS_BASE_LOGGER_NAME "-Core", LogSeverity::Trace);
		return instance;
	}

	LogSystem& LogSystem::GetClientInstance()
	{
		static LogSystem instance(CPPTOOLS_BASE_LOGGER_NAME "-Client", LogSeverity::Trace);
		return instance;
	}

	void LogSystem::SetSeverity(LogSeverity severityMin)
	{
		m_SeverityMin = severityMin;
	}

	void LogSystem::SetBaseFormat(std::string_view basetFmt)
	{
		Fmt::FormatInChar<char>(m_FmtBuffer, "{{color}{}", basetFmt);
	}

	void LogSystem::SetFormat(std::string_view basetFmt)
	{
		Fmt::FormatInChar<char>(m_FmtBuffer, "{}", basetFmt);
	}

	void LogSystem::ResetFormat()
	{
		SetBaseFormat("[{T:%h:%m:%s:%ms}] {name} >> {data}");
	}

	void LogSystem::SetName(const std::string& name) {
		m_Name = name;
	}

	void LogSystem::SetName(std::string&& name)
	{
		m_Name = std::move(name);
	}

}