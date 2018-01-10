@echo off
rem Builds main.cpp against SDL and other dependencies, using command line
rem invocations of cl.exe and link.exe
set INCLUDE=%INCLUDE%;%SDL_ROOT%\include;%GLEW_ROOT%\include
set LIB=%LIB%;%SDL_ROOT%\VisualC\x64\Debug;%GLEW_ROOT%\lib\Debug\x64
cl /c /EHsc /nologo main.cpp
link /nologo /OUT:basic.exe /SUBSYSTEM:CONSOLE SDL2.lib opengl32.lib glew32d.lib SDL2main.lib main.obj
copy %SDL_ROOT%\VisualC\x64\Debug\SDL2.dll .
copy %GLEW_ROOT%\bin\Debug\x64\glew32d.dll .
