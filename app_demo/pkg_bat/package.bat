@echo off  

cd ..
for %%i in (.) do set "folder_name=%%~ni"    
del *.apk
rmdir /S /Q "pkg" 
mkdir "pkg" 
xcopy /E /I "release\bin" "pkg\bin"
xcopy /E /I "lib" "pkg\lib"
xcopy /E /I "res" "pkg\res"
xcopy "ympkg.xml" "pkg\"
cd pkg
zip -r .\pkgtest.zip  .\bin .\lib .\res ympkg.xml
move .\pkgtest.zip ..\pkgtest.zip
cd ..
for /F "tokens=1-4 delims=/ " %%a in ('date /T') do (  
    set "year=%%b"  
    set "month=%%c"  
    set "day=%%d"  
)  
for /F "tokens=1-2 delims=: " %%a in ('time /T') do (  
    set "hour=%%a"  
    set "minute=%%b"  
)  
set "timestamp=%year%%month%%day%%hour%%minute%"
move .\pkgtest.zip  .\%folder_name%.apk 
rmdir /S /Q "pkg" 