@echo on
@echo %1 is chk or fre 
@echo %2 is WXP or W2K or WNET.
@echo %3 is copy source file path
@echo %4 is copy target dir

call set_wdk_env.bat

rem if "%4"=="/a" call wdk_clean %1 %2

pushd.
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2
popd

@echo on
build

copy %3 %4