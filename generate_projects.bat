@echo off
echo.
echo    ###########################
echo    # Project Files Generator #
echo    ###########################
echo.
call "tools/SharpMake/bin/Sharpmake.Application.exe" /sources(@"./tools/BuildScripts/main.sharpmake.cs")
echo.
echo Done.
echo.
PAUSE