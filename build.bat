@echo off
mkdir build
pushd build

rem Files
rem LibsPath
rem Libs
rem Includes

set Files= ..\*.cpp ../glad.c ..\Libs\include\imgui\*.cpp
set Includes= ..\Libs\include 
set Includes2= ..\Libs\include\imgui
set LibsPath=/LIBPATH:"..\Libs\lib\"
set SubsystemWindows=windows
set SubsystemConsole=console
set Libs=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glfw3.lib

rem Compile
set Compile=cl /DDEBUG /FeCardGame.exe /MD -FC -Zi %Files% /I  %Includes% /I %Includes2% /link %LibsPath% %Libs% /NODEFAULTLIB:MSVCRTD -subsystem:%SubsystemWindows%
%Compile%

popd 