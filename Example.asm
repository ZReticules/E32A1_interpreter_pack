include "ISA.inc"
include "uio.inc"

entry main

main:
	; ccall print, .str addr  		; ����� ��������� ������
	mov r:0, main addr
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	hlt

; .str db "������", 0
align 4

