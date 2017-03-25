@if exist "Release\Demo.exe" (
  @del Release\Demo.exe
)
@if "%DevEnvDir%" == "" (
  @call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
)
@set "solution=Demo.sln"
@MSBuild.exe -nologo %solution% /p:configuration=Release
@echo errorlevel = %errorlevel%
@if errorlevel 1 goto fail
cd Release
Demo.exe
goto end
pause
:fail
pause
:end
exit /b 2