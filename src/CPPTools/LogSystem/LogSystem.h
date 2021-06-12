	#pragma once

#include <iostream>
#include "CPPTools/Formater/Formater.h"

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
		LogSystem();
		LogSystem(const std::string& name, LogSeverity severityMin = LogSeverity::Trace, std::ostream& stream = std::cout);
		LogSystem(std::string&& name, LogSeverity severityMin = LogSeverity::Trace, std::ostream& stream = std::cout);
		~LogSystem();

	public:
		static LogSystem& GetCoreInstance();
		static LogSystem& GetClientInstance();

	public:
		void SetSeverity(LogSeverity severityMin);
		void SetName(const std::string& name);
		void SetName(std::string&& name);
		void SetBaseFormat(const char* const fmt);
		// You need to put {color} flag to have color ; use SetBaseColor instead
		void SetFormat(const char* const fmt);
		void ResetFormat();

	public:

		/////---------- Logger Severity with array as format ----------/////

		template<size_t FORMAT_SIZE, typename ...Args>
		void Log(LogSeverity severity, const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogTrace(const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogInfo(const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogWarn(const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogError(const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogFatal(const char (&format)[FORMAT_SIZE], Args&& ...args) const;

		/////---------- Logger Severity with string as format ----------/////

		template<typename ...Args>
		void Log(LogSeverity severity, const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogTrace(const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogInfo(const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogWarn(const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogError(const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogFatal(const std::string& format, Args&& ...args) const;
		 

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

		template<size_t FORMAT_SIZE, typename ...Args>
		void Log(LogStatus status, const char (&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogOk(const char(&format)[FORMAT_SIZE], Args&& ...args) const;
		template<size_t FORMAT_SIZE, typename ...Args>
		inline void LogFail(const char(&format)[FORMAT_SIZE], Args&& ...args) const;


		/////---------- Logger Status with string as format----------/////

		template<typename ...Args>
		void Log(LogStatus status, const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogOk(const std::string& format, Args&& ...args) const;
		template<typename ...Args>
		inline void LogFail(const std::string& format, Args&& ...args) const;


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
	template<>
	struct FormatType<LogSystem::LogSystem::LogSeverity>
	{
		static void Write(const LogSystem::LogSeverity t, Formater& formater) {
			switch(t)
			{
			case LogSystem::LogSeverity::Trace:
				FormatType<AnsiColorFG>::Write(AnsiColorFG::BrightBlack, formater);
				break;
			case LogSystem::LogSeverity::Info:
				FormatType<AnsiColorFG>::Write(AnsiColorFG::Green, formater);
				break;
			case LogSystem::LogSeverity::Warn:
				FormatType<AnsiColorFG>::Write(AnsiColorFG::Yellow, formater);
				break;
			case LogSystem::LogSeverity::Error:
				FormatType<AnsiColorFG>::Write(AnsiColorFG::Red, formater);
				break;
			case LogSystem::LogSeverity::Fatal:
				FormatType<AnsiColorFG>::Write(AnsiColorFG::BrightMagenta, formater);
				break;
			}
		}
	};

	template<>
	struct FormatType<LogSystem::LogStatus>
	{
		static void Write(const LogSystem::LogStatus status, Formater& formater) {
			if (status == LogSystem::LogStatus::OK)
				formater.LittleFormat("[{:C:green}]", " OK ");
			else if (status == LogSystem::LogStatus::FAIL)
				formater.LittleFormat("[{:C:red}]", "FAIL");
		}
	};
}


namespace CPPTools {

	/////---------- Logger Severity with array as format ----------/////

	template<size_t FORMAT_SIZE, typename ...Args>
	void LogSystem::Log(LogSeverity severity, const char (&format)[FORMAT_SIZE], Args&& ...args) const {
		if (severity >= m_SeverityMin) {
			char formatBuffer[500];
			Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_CSTR("color", "{color}"), FORMAT_CSTR("name", m_Name), FORMAT_CSTR("data", format));
			Fmt::FilePrintLn<500>(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_CSTR("color", severity));
		}
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogTrace(const char(&format)[FORMAT_SIZE], Args&& ...args) const {
		Log(LogSeverity::Trace, format, std::forward<Args>(args)...);
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogInfo(const char(&format)[FORMAT_SIZE], Args&& ...args) const  {
		Log(LogSeverity::Info, format, std::forward<Args>(args)...);
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogWarn(const char(&format)[FORMAT_SIZE], Args&& ...args) const {
		Log(LogSeverity::Warn, format, std::forward<Args>(args)...);
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogError(const char(&format)[FORMAT_SIZE], Args&& ...args) const  {
		Log(LogSeverity::Error, format, std::forward<Args>(args)...);
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogFatal(const char(&format)[FORMAT_SIZE], Args&& ...args) const  {
		Log(LogSeverity::Fatal, format, std::forward<Args>(args)...);
	}


	/////---------- Logger Severity with string as format ----------/////

	template<typename ...Args>
	void LogSystem::Log(LogSeverity severity, const std::string& format, Args&& ...args) const {
		if (severity >= m_SeverityMin) {
			char formatBuffer[500];
			Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_CSTR("color", "{color}"), FORMAT_CSTR("name", m_Name), FORMAT_CSTR("data", format));
			Fmt::FilePrintLn<500>(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_CSTR("color", severity));
		}
	}

	template<typename ...Args>
	inline void LogSystem::LogTrace(const std::string& format, Args&& ...args) const {
		Log(LogSeverity::Trace, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogInfo(const std::string& format, Args&& ...args) const {
		Log(LogSeverity::Info, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogWarn(const std::string& format, Args&& ...args) const {
		Log(LogSeverity::Warn, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogError(const std::string& format, Args&& ...args) const {
		Log(LogSeverity::Error, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogFatal(const std::string& format, Args&& ...args) const {
		Log(LogSeverity::Fatal, format, std::forward<Args>(args)...);
	}


	/////---------- NO-FORMAT Logger Severity ----------/////

	template<typename T>
	void LogSystem::Log(LogSeverity severity, T&& t) const {
		if (severity >= m_SeverityMin)
			Fmt::FilePrintLn<500>(m_Stream, m_FmtBuffer, FORMAT_CSTR("data", t), FORMAT_CSTR("color", severity), FORMAT_CSTR("name", m_Name));
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

	template<size_t FORMAT_SIZE, typename ...Args>
	void LogSystem::Log(LogStatus status, const char(&format)[FORMAT_SIZE], Args&& ...args) const {
		char formatBuffer[500];
		Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_CSTR("color", "{color}"), FORMAT_CSTR("name", m_Name), FORMAT_CSTR("data", format));
		Fmt::FilePrintLn<500>(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_CSTR("color", status));
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogOk(const char(&format)[FORMAT_SIZE], Args&& ...args) const {
		Log(LogStatus::OK, format, std::forward<Args>(args)...);
	}

	template<size_t FORMAT_SIZE, typename ...Args>
	inline void LogSystem::LogFail(const char(&format)[FORMAT_SIZE], Args&& ...args) const {
		Log(LogStatus::FAIL, format, std::forward<Args>(args)...);
	}


	/////---------- Logger Status with string as format----------/////

	template<typename ...Args>
	void LogSystem::Log(LogStatus status, const std::string& format, Args&& ...args) const {
		char formatBuffer[500];
		Fmt::FormatInChar(formatBuffer, m_FmtBuffer, FORMAT_CSTR("color", "{color}"), FORMAT_CSTR("name", m_Name), FORMAT_CSTR("data", format));
		Fmt::FilePrintLn<500>(m_Stream, formatBuffer, std::forward<Args>(args)..., FORMAT_CSTR("color", status));
	}

	template<typename ...Args>
	inline void LogSystem::LogOk(const std::string& format, Args&& ...args) const {
		Log(LogStatus::OK, format, std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void LogSystem::LogFail(const std::string& format, Args&& ...args) const {
		Log(LogStatus::FAIL, format, std::forward<Args>(args)...);
	}


	/////---------- NO-FORMAT Logger Status ----------/////

	template<typename T>
	void LogSystem::Log(LogStatus status, T&& t) const {
		Fmt::FilePrintLn<500>(m_Stream, m_FmtBuffer, FORMAT_CSTR("data", t), FORMAT_CSTR("color", status), FORMAT_CSTR("name", m_Name));
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


