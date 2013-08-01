
.code
ClearWriteProtect proc
		push  rax;
		mov   rax, cr0;
		and   rax, not 10000h;		//and     eax, not 10000h
		mov   cr0, rax;
		pop   rax;
ret
ClearWriteProtect ENDP

SetWriteProtect proc
		push  rax;
		mov   rax, cr0;
		or    rax, 10000h;  //or eax, 10000h
		mov   cr0, rax;
		pop   rax;
ret
SetWriteProtect ENDP

end