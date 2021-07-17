
#include "CPPToolspch.h"
#include "Profiling.h"

namespace CPPTools::Instrumentation {

	ProfileResult::ProfileResult(Profiler& link, const std::string& name)
		: m_Link(link), m_Name(name), m_Start(0), m_End(0)
	{
		std::replace(m_Name.begin(), m_Name.end(), '"', '\'');
		m_Start = Profiler::GetMicroseconds();
		m_TID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	}

	ProfileResult::ProfileResult(Profiler& link, std::string&& name)
		: m_Link(link), m_Name(std::move(name)), m_Start(0), m_End(0)
	{
		std::replace(m_Name.begin(), m_Name.end(), '"', '\'');
		m_Start = Profiler::GetMicroseconds();
		m_TID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	}

	ProfileResult::~ProfileResult() {
		if (!m_IsStoped)
			Stop();
	}

	void ProfileResult::Stop() {
		m_End = Profiler::GetMicroseconds();

		double micro = m_End - m_Start;
		double millis = micro / 1000;
		double sec = millis / 1000;

		if (sec > 1.5f)				m_Link.GetLogger().LogTrace("{} : {} seconds", m_Name, sec);
		else if (millis > 5.0f)		m_Link.GetLogger().LogTrace("{} : {} ms", m_Name, millis);
		else						m_Link.GetLogger().LogTrace("{} : {} us", m_Name, micro);

		m_Link.WriteProfile(this);
		m_IsStoped = true;
	}







	Profiler::Profiler(const std::string& name)
		: m_Name(name), m_File(m_Name + ".json", std::ios::out), m_Logger(m_Name), m_Start(GetMicroseconds()), m_IsEnd(false), m_ProfilesCount(0)
	{
		std::replace(m_Name.begin(), m_Name.end(), '"', '\'');
		WriteHeaderFile();
	}

	Profiler::~Profiler() {
		if (!m_IsEnd)
			EndSession();
	}

	void Profiler::WriteProfile(const ProfileResult* const result) {
		WriteProfile(result->GetName(), result->GetStart(), result->GetDuration(), result->GetTID());
	}

	void Profiler::WriteProfile(const std::string& name, const double start, const double dur, const std::size_t tid)
	{
		const char* newLine = m_ProfilesCount++ ? ",\n" : "";

		Fmt::FilePrint(m_File,
			R"({0}{"cat":"function","dur":{1},"name":"{2}","ph":"X","pid":{3},"tid":{4},"ts":{5}})",
			newLine,
			dur,
			name,
			0,
			tid,
			start
		);
	}

	void Profiler::EndSession() {
		WriteProfile(m_Name, m_Start, GetMicroseconds() - m_Start, 0);
		WriteFooter();

		m_File.close();

		m_IsEnd = true;
	}
	
	double Profiler::GetMicroseconds()
	{
		return (double)std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() / 1000;
	}

	void Profiler::WriteHeaderFile() {
		Fmt::FilePrint(m_File, "{\"otherData\": {},\"traceEvents\":[\n");
	}

	void Profiler::WriteFooter() {
		Fmt::FilePrint(m_File, "]}");
	}

}