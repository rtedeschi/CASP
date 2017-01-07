@echo off

IF NOT "%1"=="off" (
	set log=1
) ELSE (
	set log=0
)

set type=major_builds
call tools/build_tools.bat