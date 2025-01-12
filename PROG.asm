include "ISA.inc"
include "uio.inc"

entry main

main:
	ccall input, .input addr, 10  	; получение строки с uart
	mov r:0, 0xA  					; перенос строки
	outu r:0
	ccall print, .input addr  		; вывод считанной строки
	hlt

.input db 11 dup(?)
align 4

