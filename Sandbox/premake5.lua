
project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs {
        "src/"
    }

    IncludeAndLinkProject("CPPTools")

    filter "system:windows"
        systemversion "10.0.18362.0"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
