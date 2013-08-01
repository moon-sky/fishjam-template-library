@echo in wdk_clean.bat

rem if exist Debug rd /s /q Debug
rem if exist Release rd /s /q Release
rem if exist obj%1_%2_x86 rd /s /q obj%1_%2_x86



pushd.
rem call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
popd

rem ml64 /c AsmHelperFun.asm

@echo on
build /clean