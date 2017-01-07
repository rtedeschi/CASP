@echo off

:: Requires variable TYPE to be set to either major_builds or minor_builds before execution

IF "%type%"=="" (
	@echo.
	@echo Build type was not specified
	@echo.
	goto end
)

call set year=%%DATE:~-4%%
call set month=%%DATE:~4,2%%
call set day=%%DATE:~7,2%%
call set hour=%%TIME:~0,2%%
call set minute=%%TIME:~3,2%%
call set second=%%TIME:~6,2%%
set hour=%hour: =0%
set filename=build%month%%day%%year%%hour%%minute%%second%
set timestamp=%month%-%day%-%year% %hour%:%minute%:%second%
set build_path=dist/%type%

IF "%FrameworkDir%"=="" ( 
	@echo.
	@echo Initializing MS Visual Studio Build Tools
	@echo.
	call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat" 
)

nmake /NOLOGO
call set error=%%ERRORLEVEL%%

IF "%error%"=="0" (
	if "%log%"=="1" (
		xcopy "CASP.exe" "%build_path%/%filename%.exe*" /F /Y /Q
		echo "dist/%filename%.exe">"logs/%type%/%filename%.txt"
		echo by %USERNAME%>>"logs/%type%/%filename%.txt"
		echo on %timestamp%>>"logs/%type%/%filename%.txt"
	)
	echo.
	echo Build Successful! ^:^)
	goto end
) ELSE (
	echo.
	echo Build Error! ^:^(
	goto end
)

:end

for %%x in (%cmdcmdline%) do if /i "%%~x"=="/c" set DirectExecute=1
if defined DirectExecute pause