include "ISA.inc"
include "uio.inc"

entry main

main:
	ccall print, .str addr  		; ����� ��������� ������
	hlt

.str db "������", 0
align 4

