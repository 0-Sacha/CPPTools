

workspace "CPPTools"
	platforms { "x64", "Win32" }

	configurations {
		"Debug",
		"Release"
	}

	startproject "Sandbox"

	flags "MultiProcessorCompile"


include "Helper.lua"

-- Include directories relative to WKS
Project["CPPTools"] = "%{wks.location}/CPPTools/"

include "CPPTools"
include "Sandbox"