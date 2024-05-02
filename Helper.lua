

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

ProjectName             = ""
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


-- Clean Function --

function CleanMakefiles()
    print(" -> rm Makefiles")
    os.remove("./Makefile")
    if Project ~= nil then
        for name,path in pairs(Project) do
            print("rm Makefile : " .. name)
            os.remove("./" .. name .. "/Makefile")
        end
    end
end

function CleanVSfiles()
    print(" -> rm VS files:")
    os.rmdir("./.vs")
    os.remove("./" .. ProjectName .. ".sln")
    if Project ~= nil then
        for name, path in pairs(Project) do
            print("rm VS files : " .. name)
            os.remove("./" .. name .. "/" .. name .. ".vcxproj")
            os.remove("./" .. name .. "/" .. name .. ".vcxproj.filters")
            os.remove("./" .. name .. "/" .. name .. ".vcxproj.user")
        end
    end
end

function CleanRMfiles()
    print(" -> rm bin folder")
    os.rmdir("./bin")
    os.rmdir("./bin-int")
end

newaction {
    trigger     = "clean",
    description = "clean the software",
    execute     = function ()
       print("clean the build...")
       CleanRMfiles()
       CleanVSfiles()
       CleanMakefiles()
       print("done.")
    end
 }