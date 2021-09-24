

------ Path ------

outputdir = "%{cfg.system}-%{_ACTION}/%{cfg.buildcfg}-%{cfg.architecture}"


------ Filter ------

filter "platforms:x86"
    defines "BASE_ARCHITECTURE_X86"
    architecture "x86"

filter "platforms:x64"
    defines "BASE_ARCHITECTURE_X64"
    architecture "x64"

filter "configurations:Debug"
    defines "BASE_TARGET_DEBUG"

filter "configurations:Release"
    defines "BASE_TARGET_RELEASE"

filter "action:gmake2"
    defines "BASE_COMPILER_GCC"

filter "action:vs*"
    defines "BASE_COMPILER_MSVC"


------ Include define and link ------

Project                 = {}
ProjectPublicIncludes   = {}
ProjectPublicDefines    = {}
ProjectPublicLinks      = {}


function IncludeProject(str)
    if ProjectPublicIncludes ~= nil then
        if ProjectPublicIncludes[str] ~= nil then
            for _, value in ipairs(ProjectPublicIncludes[str]) do
                includedirs (value)
            end
        end
    end

    if ProjectPublicDefines ~= nil then
        if ProjectPublicDefines[str] ~= nil then
            for _, value in ipairs(ProjectPublicDefines[str]) do
                defines (value)
            end
        end
    end

    if ProjectPublicLinks ~= nil then
        if ProjectPublicLinks[str] ~= nil then
            for _, value in ipairs(ProjectPublicLinks[str]) do
                links (value)
            end
        end
    end
end


function IncludeAndLinkProject(str)
    if ProjectPublicIncludes ~= nil then
        if ProjectPublicIncludes[str] ~= nil then
            for _, value in ipairs(ProjectPublicIncludes[str]) do
                includedirs (value)
            end
        end
    end

    if ProjectPublicDefines ~= nil then
        if ProjectPublicDefines[str] ~= nil then
            for _, value in ipairs(ProjectPublicDefines[str]) do
                defines (value)
            end
        end
    end

    if ProjectPublicLinks ~= nil then
        if ProjectPublicLinks[str] ~= nil then
            for _, value in ipairs(ProjectPublicLinks[str]) do
                links (value)
            end
        end
    end

    links (str)
end