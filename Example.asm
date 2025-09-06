include "ISA.inc"
include "uio.inc"

entry main

main:
	; ccall print, .str addr  		; вывод считанной строки
	mov r:0, main addr
	hlt

; .str db "Привет", 0
align 4

