
.code

GetKiSystemCall64Address proc
	xor rax, rax
	mov ecx, 0C0000082h
	rdmsr			; read MSR(model specific register)
	shl rdx, 20h
	or rax, rdx
	ret
GetKiSystemCall64Address ENDP

SetSoftBreakPoint proc
	int 3
	ret
SetSoftBreakPoint ENDP


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