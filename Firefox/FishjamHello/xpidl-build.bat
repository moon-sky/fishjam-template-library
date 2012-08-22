rem %1 -- XPSDK_DIR Path

..\xpidl.exe -w -v -m header  -I "%1idl" FishjamHello.idl
..\xpidl.exe -w -v -m typelib -I "%1idl" FishjamHello.idl

copy FishjamHello.xpt ..\Output\components\FishjamHello.xpt

rem %1 -w -v -m header -I %2 %3%4.idl
rem %1 -w -v -m typelib -I %2 %3%f.idl
rem copy %3%4.xpt %5

rem "$(SolutionDir)xpidl.exe" -m header -I "$(XPSDK_DIR)idl" -w -v "$(InputPath)" 
rem "$(SolutionDir)xpidl.exe" -m typelib -I "$(XPSDK_DIR)idl" -w -v "$(InputPath)" 

rem del NaverToolbar.xpt NaverToolbar.h NaverToolbar.xpt
rem gecko_sdk\xpidl.exe -m header -I..\..\..\..\..\gecko_sdk\xulrunner-14.0.1.en-US.win32.sdk\xulrunner-sdk\idl ..\base\NaverToolbar.idl
rem gecko_sdk\xpidl.exe -m typelib -I..\..\..\..\..\gecko_sdk\xulrunner-14.0.1.en-US.win32.sdk\xulrunner-sdk\idl ..\base\NaverToolbar.idl

rem copy NaverToolbar.xpt ..\..\..\components\NaverToolbar.xpt