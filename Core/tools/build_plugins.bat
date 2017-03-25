@echo off
setlocal enabledelayedexpansion

For /R ./plugins %%G in (*.cpp) do (
    for %%F in (%%G) do set pluginFilename=%%~nF
    @cl /c "%%G" /Fo"obj/Plugin_!pluginFilename!.o"
)