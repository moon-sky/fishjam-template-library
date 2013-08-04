
.code

%ifdef _M_IX86

SetSoftBreakPoint proc
	int 3
	ret
SetSoftBreakPoint ENDP

ClearWriteProtect proc
		push  eax;
		mov   eax, cr0;
		and   eax, not 10000h;		//and     eax, not 10000h
		mov   cr0, eax;
		pop   eax;
ret
ClearWriteProtect ENDP

SetWriteProtect proc
		push  eax;
		mov   eax, cr0;
		or    eax, 10000h;  //or eax, 10000h
		mov   cr0, eax;
		pop   eax;
ret
SetWriteProtect ENDP

%endif   ;_M_IX86

end