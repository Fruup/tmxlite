project "TmxLite"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	characterset "MBCS"

	targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
	objdir "bin-int/%{prj.name}/%{cfg.buildcfg}"

	files {
		"tmxlite/include/tmxlite/**",
		"tmxlite/src/**.hpp",
		"tmxlite/src/**.h",
		"tmxlite/src/**.cpp",
		"tmxlite/src/**.c"
	}

	includedirs {
		"tmxlite/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "HZ_DEBUG", "ENET_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "HZ_RELEASE" }
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines { "HZ_DIST" }
		runtime "Release"
		optimize "on"
