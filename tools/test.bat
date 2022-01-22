@echo off

if [%~1]==[] (cd build\tests) else (cd build_%~1\tests)

ctest
cd ..\..