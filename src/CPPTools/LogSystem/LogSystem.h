#pragma once

#include <iostream>
#include "CPPTools/Formatter/Formatter.h"

#ifndef CPPTOOLS_BASE_LOGGER_NAME
	#define CPPTOOLS_BASE_LOGGER_NAME "APP"
#endif

namespace CPPTools {

	class LogSystem {
	public:
		enum class LogSeverity : int {
			Trace,
			Info,
			Warn,
			Error,
			Fatal
		};

		enum class LogStatus : int {
			OK,
			FAIL
		};

	public:
		LogSystem() : m_Name("Logger"), m_SeverityMin(LogSeverity::Trace), m_Stream(std::cout) { ResetFormat(); }
		explicit LogSystem(const std::string& name, LogSeverity severityMin = LogSeverity::Trace, std::ostream& stream = std::cout) : m_Name(name), m_SeverityMin(severityMin), m_Stream(stream)			{ ResetFormat(); }
		explicit LogSystem(std::string&& name, LogSeverity severityMin = LogSeverity::Trace, std::ostream& stream = std::cout)		: m_Name(std::move(name)), m_SeverityMin(severityMin), m_Stream(stream) { ResetFormat(); }
		~LogSystem() = default;

	public:
		static LogSystem& GetCoreInstance()				{ static LogSystem instance(CPPTOOLS_BASE_LOGGER_NAME "-Client", LogSeverity::Trace); return instance; }
		static LogSystem& GetClientInstance()			{ static LogSystem instance(CPPTOOLS_BASE_LOGGER_NAME "-Core", LogSeverity::Trace); return instance; }

	public:
		void SetSeverity(LogSeverity severityMin)		{ m_SeverityMin = severityMin; }
		void SetName(const std::string& name)			{ m_Name = name; }
		void SetName(std::string&& name)				{ m_Name = std::move(name); }
		void SetBaseFormat(std::string_view basetFmt)	{ Fmt::FormatInChar(m_FmtBuffer, "{color}{}", basetFmt); }
		// You need to put {color} flag to have color ; use SetBaseColor instead
		void SetFormat(std::string_view basetFmt)		{ Fmt::FormatInChar(m_FmtBuffer, basetFmt); }
		void ResetFormat()								{ SetBaseFormat("[{T:%h:%m:%s:%ms}] {name} >> {data}"); }

	public:

		/////---------- Logger Severity with array as format ----------/////

		template<typename ...Args>
		void Log(LogSeverity severity, const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogTrace(const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogInfo(const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogWarn(const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogError(const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogFatal(const std::string_view format, Args&& ...args) const;

		/////---------- NO-FORMAT Logger Severity ----------/////

		template<typename T>
		void Log(LogSeverity severity, T&& t) const;
		template<typename T>
		inline void LogTrace(T&& t) const;
		template<typename T>
		inline void LogInfo(T&& t) const;
		template<typename T>
		inline void LogWarn(T&& t) const;
		template<typename T>
		inline void LogError(T&& t) const;
		template<typename T>
		inline void LogFatal(T&& t) const;

	public:
		/////---------- Logger Status with array as format ----------/////

		template<typename ...Args>
		void Log(LogStatus status, const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogOk(const std::string_view format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogFail(const std::string_view format, Args&& ...args) const;

		/////---------- NO-FORMAT Logger Status ----------/////

		template<typename T>
		void Log(LogStatus status, T&& t) const;
		template<typename T>
		inline void LogOk(T&& t) const;
		template<typename T>
		inline void LogFail(T&& t) const;

	private:
		std::string m_Name;
		LogSeverity m_SeverityMin;
		std::ostream& m_Stream;

		char m_FmtBuffer[50];
	};
}


namespace CPPTools::Fmt {
	template<typename FormatContext>
	struct FormatType<LogSystem::LogSeverity, FormatContext>
	{
		static void Write(const LogSystem::LogSeverity t, FormatContext& context) {
			switch (t)
			{
			case LogSystem::LogSeverity::Trace:
				FormatType<Detail::AnsiColorFG, FormatContext>::Write(Detail::AnsiColorFG::BrightBlack, context);
				break;
			case LogSystem::LogSeverity::Info:
				FormatType<Detail::AnsiColorFG, FormatContext>::Write(Detail::AnsiColorFG::Green, context);
				break;
			case LogSystem::LogSeverity::Warn:
				FormatType<Detail::AnsiColorFG, FormatContext>::Write(Detail::AnsiColorFG::Yellow, context);
				break;
			case LogSystem::LogSeverity::Error:
				FormatType<Detail::AnsiColorFG, FormatContext>::Write(Detail::AnsiColorFG::Red, context);
				break;
			case LogSystem::LogSeverity::Fatal:
				FormatType<Detail::AnsiColorFG, FormatContext>::Write(Detail::AnsiColorFG::BrightMagenta, context);
				break;
			}
		}
	};

	template<typename FormatContext>
	struct FormatType<LogSystem::LogStatus, FormatContext>
	{
		static void Write(const LogSystem::LogStatus status, FormatContext& context) {
			if (status == LogSystem::LogStatus::OK)			context.LittleFormat("[{:C:green}]", " OK ");
			else if (status == LogSystem::LogStatus::FAIL)	context.LittleFormat("[{:C:red}]", "FAIL");
		}
	};
}


namespace CPPTools {

	/////---------- Logger Severity with array as format ----------/////

	template<typename ...Args>
	void LogSystem::Log(LogSeverity severity, const std::string_view format, Args&& ...args) const {
		if (severity >= m_SeverityMin) {
			char formatBuffer[1024];
			Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_SV("name", m_Name), FORMAT_SV("data", format));
			Fmt::FilePrintLn(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_SV("color", severity));
		}
	}

	template<typename ...Args>
	inline void LogSystem::LogTrace(const std::string_view format, Args&& ...args) const {
		Log(LogSeverity::Trace, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogInfo(const std::string_view format, Args&& ...args) const  {
		Log(LogSeverity::Info, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogWarn(const std::string_view format, Args&& ...args) const {
		Log(LogSeverity::Warn, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogError(const std::string_view format, Args&& ...args) const  {
		Log(LogSeverity::Error, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogFatal(const std::string_view format, Args&& ...args) const  {
		Log(LogSeverity::Fatal, format, std::forward<Args>(args)...);
	}

	/////---------- NO-FORMAT Logger Severity ----------/////

	template<typename T>
	void LogSystem::Log(LogSeverity severity, T&& t) const {
		if (severity >= m_SeverityMin)
			Fmt::FilePrintLn(m_Stream, m_FmtBuffer, FORMAT_SV("data", t), FORMAT_SV("color", severity), FORMAT_SV("name", m_Name));
	}

	template<typename T>
	inline void LogSystem::LogTrace(T&& t) const {
		Log(LogSeverity::Trace, std::forward<T>(t));
	}

	template<typename T>
	inline void LogSystem::LogInfo(T&& t) const {
		Log(LogSeverity::Info, std::forward<T>(t));
	}

	template<typename T>
	inline void LogSystem::LogWarn(T&& t) const {
		Log(LogSeverity::Warn, std::forward<T>(t));
	}

	template<typename T>
	inline void LogSystem::LogError(T&& t) const {
		Log(LogSeverity::Error, std::forward<T>(t));
	}

	template<typename T>
	inline void LogSystem::LogFatal(T&& t) const {
		Log(LogSeverity::Fatal, std::forward<T>(t));
	}

}

namespace CPPTools {

	/////---------- Logger Status with array as format ----------/////

	template<typename ...Args>
	void LogSystem::Log(LogStatus status, const std::string_view format, Args&& ...args) const {
		char formatBuffer[500];
		Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_SV("name", m_Name), FORMAT_SV("data", format));
		Fmt::FilePrintLn(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_SV("color", status));
	}

	template<typename ...Args>
	inline void LogSystem::LogOk(const std::string_view format, Args&& ...args) const {
		Log(LogStatus::OK, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogFail(const std::string_view format, Args&& ...args) const {
		Log(LogStatus::FAIL, format, std::forward<Args>(args)...);
	}


	/////---------- NO-FORMAT Logger Status ----------/////

	template<typename T>
	void LogSystem::Log(LogStatus status, T&& t) const {
		Fmt::FilePrintLn(m_Stream, m_FmtBuffer, FORMAT_SV("data", t), FORMAT_SV("color", status), FORMAT_SV("name", m_Name));
	}

	template<typename T>
	inline void LogSystem::LogOk(T&& t) const {
		Log(LogStatus::OK, std::forward<T>(t));
	}

	template<typename T>
	inline void LogSystem::LogFail(T&& t) const {
		Log(LogStatus::FAIL, std::forward<T>(t));
	}
}


