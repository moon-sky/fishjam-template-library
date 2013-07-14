@echo on
@echo %1 is chk or fre %2 is WXP or W2K or WNET.

call set_wdk_env.bat

if "%4"=="/a" call wdk_clean %1 %2

pushd.
call %BASEDIR%\bin\setenv.bat %BASEDIR% %1 %2
popd

@echo on
build