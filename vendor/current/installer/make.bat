@echo off
cls
echo This will create the various installers for a TA Spring release
echo .
echo Remember to generate all needed path information for the maps to be included
echo Press any key to continue..
echo .
echo Also remember to update the version string in Lesson2.cpp so that
echo the correct .pdb can be identified!
echo.
pause > nul

echo Creating normal installer
"C:\Program Files\NSIS\makensis.exe" /V3 taspring.nsi 

echo Creating update installer
"C:\Program Files\NSIS\makensis.exe" /V3 /DSP_UPDATE taspring.nsi  

rem echo Creating installer without cavedog content
rem "C:\Program Files\NSIS\makensis.exe" /V3 /DSP_ALLMAPS taspring.nsi 

rem echo Creating small installer without cavedog content
rem "C:\Program Files\NSIS\makensis.exe" /V3 taspring.nsi 

rem echo Creating core only installer
rem "C:\Program Files\NSIS\makensis.exe" /V3 /DSP_CORE taspring.nsi 

echo Copying debug information
copy ..\rts\release\spring.pdb .

echo All done.. Matching debug information copied to installer folder
echo If this is a public release, make sure to save this and tag CVS etc..
echo Press any key to continue..
pause > nul