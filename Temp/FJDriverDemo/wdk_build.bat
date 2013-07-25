@echo on
@echo %1 is chk or fre 
@echo %2 is x86 / ia64 / x64
@echo %3 is WIN7 / WLH / WXP / W2K or WNET
@echo %4 is copy source directory path
@echo %5 is copy source file(with out extersion name)
@echo %5 is copy target dir


call set_wdk_env.bat

rem if "%6"=="/a" call wdk_clean %1 %2

pushd.
rem call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 
popd

@echo on
build

xcopy %4%5.sys %6 /Y
xcopy %4%5.pdb %6 /Y
