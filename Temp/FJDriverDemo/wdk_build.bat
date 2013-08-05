@echo off
rem %1 is chk or fre 
rem %2 is x86 / ia64 / x64
rem %3 is WIN7 / WLH / WXP / W2K or WNET
rem %4 is copy source directory path
rem %5 is copy source file(with out extersion name)
rem %6 is copy target dir

rem WIN32=100;_X86_=1;WINVER=0x501;DBG=1

@echo on
call set_wdk_env.bat

rem if "%7"=="/a" call wdk_clean %1 %2

pushd.
rem call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 
popd

rem ml64 /c AsmHelperFun.asm

@echo on
build

rem xcopy %4%5.sys %6 /Y
rem xcopy %4%5.pdb %6 /Y
