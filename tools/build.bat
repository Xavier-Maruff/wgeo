@REM TODO: Remove the dynamically changing build directory names - it is dumb

@echo off
SETLOCAL

if [%~1]==[] (set build_type=Release) else (set build_type=%~1)
mkdir build
cd build

:end
echo Building %build_type% distribution...
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=%build_type% -DBUILD_EXAMPLES=1 ..
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build . --config %build_type%
if %errorlevel% neq 0 exit /b %errorlevel%
xcopy ..\lib\%build_type%\* ..\lib /e /Y
rmdir ..\lib\%build_type% /s /q
cd ..
echo Done