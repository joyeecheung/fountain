-- premake5.lua
solution "Fountain"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "Fountain"
      kind "ConsoleApp"
      language "C++"
      files { "src/*.cpp" }
      libdirs { "lib" }
      includedirs { "include", "src" }
      buildoptions "-std=c++11"

      configuration { "windows" }
        links { "FreeGLUT", "glu32", "opengl32", "FreeImage"}

      configuration { "macosx" }
        linkoptions { "-framework Carbon -framework OpenGL -framework GLUT" }

      configuration { "not windows", "not macosx" }
        links { "X11", "GL", "GLU", "GLUT" }

      configuration "Debug"
         defines { "DEBUG" } -- -DDEBUG
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" } -- -NDEBUG
         flags { "Optimize" }
