!ifndef ___FUTILS_INCLUDED___
!define ___FUTILS_INCLUDED___
               
!include "WinMessages.nsh"
!include nsDialogs.nsh
!include LogicLib.nsh

;stack0 -- ControlID
Function GetControlInfo
    Exch $5
    Push $0
    Push $1
    Push $2
    Push $3
    Push $4
    Push $6
    ;MessageBox MB_OK "In GetControlInfo $5"
    GetDlgItem $0 $HWNDPARENT $5
    ${NSD_GetText} $0 $0
  	MessageBox MB_OK "Text is $\n$\n $0"
	/*
    GetDlgItem $0 $HWNDPARENT $5
    StrCmp $0 0 error
    SendMessage $0 ${WM_GETTEXTLENGTH} 0 0 $6
    System::Alloc ${NSIS_MAX_STRLEN}
    Pop $3
    SendMessage $0 ${WM_GETTEXT} ${NSIS_MAX_STRLEN} $3
    MessageBox MB_OK $3
    ;${IF} $0 != 0
    ;${Else}
    ;  MessageBox MB_OK "${Id} Handle is 0"
    ;${EndIF}
    done:
      System::Free $3
      Goto exit
    error:
      MessageBox MB_OK "Error $5 Handle is 0"
    exit:
*/
      Pop $6
      Pop $4
      Pop $3
      Pop $2
      Pop $1
      Pop $0
      Exch $5
      
FunctionEnd

!endif ;___FUTILS_INCLUDED___
