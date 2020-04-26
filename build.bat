@ECHO ON

RMDIR /Q /S build
MKDIR build
PUSHD build

@CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

cmake -G "Visual Studio 15" ..
devenv aoc-17.sln /build

CD ..