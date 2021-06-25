#pragma once

#include "CPPTools/Core/Core.h"
#include "CPPTools/LogSystem/LogSystem.h"

/**
 * __LINE__
 * __FUNCTION__
 * __FUNCDNAME__
 * __FUNCSIG__
 * __FILE__
 * __DATE__
 */


namespace CPPTools::Instrumentation {

	class Profiler;
	class ProfileResult;

	class ProfileResult {
	public:
		ProfileResult(Profiler& link, const std::string& name);
		ProfileResult(Profiler& link, std::string&& name);
		~ProfileResult();

	public:
		void Stop();
		const std::string& GetName() const;
		const float GetStart() const;
		const float GetDuration() const;
		const std::size_t GetTID() const;


	private:
		Profiler& m_Link;
		std::string m_Name;
		float m_Start, m_End;
		size_t m_TID;
		bool m_IsStoped = false;
	};



	class Profiler {
	public:
		Profiler(const std::string& name);
		~Profiler();

	public:
		void WriteProfile(const ProfileResult* const result);
		void WriteProfile(std::string name, const float start, const float dur, const std::size_t tid);
		void EndSession();

		inline const CPPTools::LogSystem& GetLogger() { return m_Logger; }

	public:
		static Profiler& GetInstance();
		static float GetMicroseconds();

	private:
		void WriteHeaderFile();
		void WriteFooter();

	private:
		std::string m_Name;
		std::ofstream m_File;
		CPPTools::LogSystem m_Logger;
		float m_Start;
		bool m_IsEnd;
		size_t m_ProfilesCount;
	};

}

#ifdef PROFILING_ENABLE

#define PROFILER_FUNCSIG_AUTO(profiler)								CPPTools::Instrumentation::ProfileResult profile##__LINE__(profiler, __FUNCSIG__)
#define PROFILER_DEFAULT_FUNCSIG_AUTO()								PROFILER_FUNCSIG_AUTO(CPPTools::Instrumentation::Profiler::GetInstance())


#define PROFILER_FUNC_AUTO(profiler)								CPPTools::Instrumentation::ProfileResult profile##__LINE__(profiler, __FUNCTION__)
#define PROFILER_DEFAULT_FUNC_AUTO()								PROFILER_FUNC_AUTO(CPPTools::Instrumentation::Profiler::GetInstance())

#define PROFILER_FUNC_FMT(profile, profiler, ...)					PROFILER_FUNC_NAME(CPPTools::Fmt::FormatString(__VA_ARGS__), profile, profiler);

#define PROFILER_DEFAULT_FUNC_FMT(profile, ...)						PROFILER_FUNC_FMT(profile, CPPTools::Instrumentation::Profiler::GetInstance(), __VA_ARGS__);

// ----------- CTools ----------- //

#define PROFILER_FUNC_NAME(name, profile, profiler)					CPPTools::Instrumentation::ProfileResult profile(profiler, name)
#define PROFILER_FUNC(profile, profiler)							PROFILER_FUNC_NAME(#profile, profile, profiler)

#define PROFILER_DEFAULT_FUNC_NAME(name, profile)					PROFILER_FUNC_NAME(name, profile, CPPTools::Instrumentation::Profiler::GetInstance())
#define PROFILER_DEFAULT_FUNC(profile)								PROFILER_DEFAULT_FUNC_NAME(#profile, profile)

#define PROFILER_CONSTRUCT(profiler)								CPPTools::Instrumentation::Profiler profiler(#profiler);
#define PROFILER_END(profiler)										profiler.EndSession()

#else // PROFILING_ENABLE

#define PROFILER_FUNC_AUTO(profiler)								
#define PROFILER_FUNC_AUTO(profiler)								
#define PROFILER_DEFAULT_FUNC_AUTO()								

#define PROFILER_FUNC_FMT(profile, profiler, fmt, ...)				
#define PROFILER_DEFAULT_FUNC_FMT(profile, fmt, ...)				

// ----------- CTools ----------- //

#define PROFILER_FUNC_NAME(name, profile, profiler)					
#define PROFILER_FUNC(profile, profiler)							

#define PROFILER_DEFAULT_FUNC_NAME(name, profile)					
#define PROFILER_DEFAULT_FUNC(profile)								

#define PROFILER_CONSTRUCT(profiler)	
#define PROFILER_END(profiler)										

#endif // PROFILING_ENABLE



#ifdef CTOOLS_PROFILER_ENABLE

#ifdef PROFILING_ENABLE

#define PROFILER_CREATE(profiler)									CPPTools::Instrumentation::Profiler profiler(#profiler);
#define PROFILER_BEGIN(profiler)									CPPTools::Instrumentation::Profiler profiler(#profiler);

#define PROFILER_DEFAULT_BEGIN()									CPPTools::Instrumentation::Profiler::GetInstance()
#define PROFILER_DEFAULT_END()										PROFILER_END(CPPTools::Instrumentation::Profiler::GetInstance())

#else // PROFILING_ENABLE

#define PROFILER_CREATE(profiler)									
#define PROFILER_BEGIN(profiler)									

#define PROFILER_DEFAULT_BEGIN()									
#define PROFILER_DEFAULT_END()										

#endif // PROFILING_ENABLE
#endif // CTOOLS_MEMORY_PROFILER_ENABLE
