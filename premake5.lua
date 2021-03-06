
function define_C()
    language "C"
end

function define_Cpp()
    language "C++"
end

function string.starts(String,Start)
   return string.sub(String,1,string.len(Start))==Start
end

function link_to(lib)
	links (lib)
	includedirs ("../"..lib)
end

function download_progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100);
    print("Download progress (" .. percent .. "%/100%)")
  end

function check_raylib()
    if(os.isdir("raylib") == false and os.isdir("raylib-master") == false) then
        print("Raylib not found, downloading from github")
        local result_str, response_code = http.download("https://github.com/raysan5/raylib/archive/refs/heads/master.zip", "raylib-master.zip", {
            progress = download_progress,
            headers = { "From: Premake", "Referer: Premake" }
        })
        print("Unzipping to " ..  os.getcwd())
        zip.extract("raylib-master.zip", os.getcwd())
        os.remove("raylib-master.zip")
    end
end

workspaceName = path.getbasename(os.getcwd())

workspace (workspaceName)
    configurations { "Debug", "Release"}
    platforms { "x64", "x86"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"    

    filter { "platforms:x64" }
        architecture "x86_64"
        
    targetdir "bin/%{cfg.buildcfg}/"
	
check_raylib();

include ("raylib_premake5.lua")

folders = os.matchdirs("*")

for _, folderName in ipairs(folders) do
	if (folderName ~= "raylib" and string.starts(folderName, "raylib") == false and string.starts(folderName, "_") == false and string.starts(folderName, ".") == false) then
        if (os.isfile(folderName .. "/premake5.lua")) then
            print(folderName)
			workspace (workspaceName)
				startproject(folderName)
			
            include (folderName)
			
        end
	end
end