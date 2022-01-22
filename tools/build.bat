@REM TODO: Remove the dynamically changing build directory names - it is dumb

@echo off
SETLOCAL

if [%~1]==[] (goto :default_build) else (goto :dist_build)

:default_build
mkdir build
cd build
set build_type=Release
goto :end

:dist_build
mkdir build_%~1
cd build_%~1
set build_type=%~1
goto :end

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