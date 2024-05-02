

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
ProjectName = "CPPTools"
Project["CPPTools"] = "%{wks.location}/CPPTools/"
Project["Sandbox"] = "%{wks.location}/Sandbox/"

include "CPPTools"
include "Sandbox"