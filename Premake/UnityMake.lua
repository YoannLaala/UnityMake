solution("UnityMake")
    configurations{ "Debug", "Release", "Master" }
    location("../Solution/")
    platforms { "x64" }
	characterset ("MBCS")

    filter { "platforms:x64" }
        architecture "x64"

    filter { "system:windows" }
        defines { "PLATFORM_PC" }

	flags { "ExtraWarnings", "FatalWarnings" }
	
	-- Define + Optimization per configuration
	filter { "configurations:Debug" }
		defines { "DEBUG" }
		optimize "Off"
		symbols "On"
		
	filter { "configurations:Release or Master"}
		defines { "NDEBUG" }
	
	filter { "configurations:Release" }
		optimize "On"
	
	filter { "configurations:Master" }
		defines { "MASTER" }
		optimize "Full"
		
	project("UnityMake")
		location("../Solution/")
		kind("ConsoleApp")

		includedirs 
		{ 
			"../Sources/"
		}
		files
		{ 
			"../Sources/**.h**",
			"../Sources/**.c**",
		}
		links
		{
			"Shlwapi.lib"
		}
		debugdir("../Binaries/")
		targetdir("../Binaries/")