@echo off
call set year=%%DATE:~-4%%
call set month=%%DATE:~4,2%%
call set day=%%DATE:~7,2%%
call set hour=%%TIME:~0,2%%
call set minute=%%TIME:~3,2%%
call set second=%%TIME:~6,2%%
set hour=%hour: =0%
set filename=build%month%%day%%year%%hour%%minute%%second%
set timestamp=%month%-%day%-%year% %hour%:%minute%:%second%
set type=minor_builds
set build_path=dist/%type%

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"
nmake /NOLOGO
call set error=%%ERRORLEVEL%%

IF "%error%"=="0" (
	echo "dist/%filename%.exe">"logs/%type%/%filename%.txt"
	echo by %USERNAME%>>"logs/%type%/%filename%.txt"
	echo on %timestamp%>>"logs/%type%/%filename%.txt"
	echo.
	echo Build Successful! ^:^)
	echo.
	goto end
) ELSE (
	echo.
	echo Build Error! ^:^(
	echo.
	goto end
)

:end
::pause