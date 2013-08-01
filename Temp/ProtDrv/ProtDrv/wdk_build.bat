rem @echo off
rem %1 is chk or fre 
rem %2 is x86 / ia64 / x64
rem %3 is WIN7 / WLH / WXP / W2K or WNET

rem DDKBUILDENV=chk;DDK_TARGET_OS=Win7;_BUILDARCH=x86

set INNER_DDKBUILDENV=%1
set INNER_DDK_TARGET_OS=%2
set INNER__BUILDARCH=%3

if "%INNER_DDKBUILDENV%"=="" set INNER_DDKBUILDENV=chk
if "%INNER_DDK_TARGET_OS%"=="" set INNER_DDK_TARGET_OS=Win7
if "%INNER__BUILDARCH"=="" set INNER__BUILDARCH=x64

rem %4 is copy source directory path
rem %5 is copy source file(with out extersion name)
rem %6 is copy target dir

rem WIN32=100;_X86_=1;WINVER=0x501;DBG=1

rem @echo on
rem call set_wdk_env.bat

rem if "%7"=="/a" call wdk_clean %1 %2

pushd.
rem call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
popd

rem ml64 /c AsmHelperFun.asm

@echo on
build

