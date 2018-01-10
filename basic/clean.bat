@echo off
rem Removes all intermediate and final MSVC build artifacts from the current
rem folder, by file extension
if exist *.exe (
    del *.exe
)
if exist *.obj (
    del *.obj
)
