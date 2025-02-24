format binary

; ��������������� ������ ��� ����, ����� �������� 
; �� ��������� ������
; ����� �����:
; ���� 0-2 - ��� �������
; ���� 3-6 - ��������
; ���� 7-11 - ������ �������-�������
; ���� ������:
;	000b - nop
; 	001b - �������, �������
; 		���� 12-31 - ������� �������(20 ���)
; 	010b - �������, �������, �������
;		���� 12-16 - ������ �������-�������
;		���� 17-31 - �������� �������(15 ���)
; 	011b - �������, �������, �������
;		���� 12-16 - ������ �������-�������
;		���� 17-21 - ������ �������-�������
; 	110b - �������, �������, �������, �������(� ��� ���������� �������)
;		���� 12-16 - ������ �������-�������
;		���� 17-21 - ������ �������-�������
;		���� 22-26 - ��������� �������-�������

macro _instr opcode, instrnum*, op1*, op2=r:0, op3=r:0, op4{
	opcode = (instrnum and 1111b) shl 3  								; ��������
	match =r:reg1, op1\{
		opcode = opcode or ((reg1 and 11111b) shl 7)
		match =r:reg2, op2\\{											; ���� ������ ������� �������
			opcode = opcode or ((reg2 and 11111b) shl 12)
			match =r:reg3, op3\\\{										; ���� ������ ������� �������
				opcode = opcode or ((reg3 and 11111b) shl 17)
				match =r:reg4, op4\\\\{									; ���� 4 �������� ���� � �� �������
					opcode = opcode or ((reg4 and 11111b) shl 22)
					opcode = opcode or 110b
				rept 0\\\\{\\\\} rept 1\\\\{ 							; ���� 4 �������� �� �������(��������� ��� ��� ���)
					opcode = opcode or 011b
				\\\\}
			rept 0\\\{\\\} rept 1\\\{ 									; ���� 3-� �������� �� �������
				opcode = opcode or ((op3 and 111111111111111b) shl 17)	; ������ �������� ������� ()
				opcode = opcode or 010b
			\\\}
		rept 0\\{\\} rept 1\\{ 											; ���� ������ ������� �� �������
			opcode = opcode or ((op2 and 11111111111111111111b) shl 12)	; ������ ������� �������
			opcode = opcode or 001b
		\\}
	\}
}

nop equ dd 0

hlt equ dd 5

macro mov args*&{
	local opcode
	_instr opcode, 0, args
	dd opcode
}

macro add args*&{
	local opcode
	_instr opcode, 1, args
	dd opcode
}

macro sub args*&{
	local opcode
	_instr opcode, 2, args
	dd opcode
}

macro not args*&{
	local opcode
	_instr opcode, 3, args
	dd opcode
}

macro and args*&{
	local opcode
	_instr opcode, 4, args
	dd opcode
}

macro or args*&{
	local opcode
	_instr opcode, 5, args
	dd opcode
}

macro xor args*&{
	local opcode
	_instr opcode, 6, args
	dd opcode
}

macro cmpa args*&{
	local opcode
	_instr opcode, 9, args
	dd opcode
}

macro cmpe args*&{
	local opcode
	_instr opcode, 10, args
	dd opcode
}

macro cmpb args*&{
	local opcode
	_instr opcode, 11, args
	dd opcode
}

macro cmovnz args*&{
	local opcode
	_instr opcode, 12, args
	dd opcode
}

macro cmovz args*&{
	local opcode
	_instr opcode, 13, args
	dd opcode
}

; ��� ��������� ������ ���� 30-31 ���������� ��� ������
macro shr args*&{
	local opcode
	_instr opcode, 7, args, r:0
	dd opcode
}

macro shl args*&{
	local opcode
	_instr opcode, 8, args, r:0
	dd opcode
}

macro sar args*&{
	local opcode
	_instr opcode, 7, args, r:0
	dd opcode or (1 shl 30)
}

macro sal args*&{
	local opcode
	_instr opcode, 8, args, r:0
	dd opcode or (1 shl 30)
}

macro shrd args*&{
	local opcode
	_instr opcode, 7, args
	dd opcode or (2 shl 30)
}

macro shld args*&{
	local opcode
	_instr opcode, 8, args
	dd opcode or (2 shl 30)
}

macro ror args*&{
	local opcode
	_instr opcode, 7, args, r:0
	dd opcode or (3 shl 30)
}

macro rol args*&{
	local opcode
	_instr opcode, 8, args, r:0
	dd opcode or (3 shl 30)
}

addr fix shr 2

; ��� ������ � ���������� ����� ������ � ������ ������ �������� ������� �����
; ���� 0-2 - ��� �������, 100b
; ���� 3-4 - ��������
; ���� 5-6 - ��� �������� ����������:
_MEM_ 	= 00b 	; ������
_UART_ 	= 01b 	; uart
_GPIO_ 	= 10b 	; �����
; ���� 7-11 - ������ �������-�������
; ���� 12-16 - ������ �������-������� 
macro _mug_ins opcode, _operation_, _out_, op1*, op2{
	opcode = 100b
	opcode = opcode or _operation_ shl 3
	opcode = opcode or _out_ shl 5
	match r:reg1, op1\{
		opcode = opcode or ((reg1 and 11111b) shl 7)
	\}
	match r:reg2, op2\{
		opcode = opcode or ((reg2 and 11111b) shl 12)
	\}
}

macro inm op1*, op2*{
	local opcode
	_mug_ins opcode, 0, _MEM_, op1, op2
	dd opcode
}

macro outm op1*, op2*{
	local opcode
	_mug_ins opcode, 1, _MEM_, op1, op2
	dd opcode
}

macro getu op1*{
	local opcode
	_mug_ins opcode, 0, _UART_, op1
	dd opcode
}

macro setu op1*{
	local opcode
	_mug_ins opcode, 1, _UART_, op1
	dd opcode
}

macro inu op1*{
	local opcode
	_mug_ins opcode, 10b, _UART_, op1
	dd opcode
}

macro outu op1*{
	local opcode
	_mug_ins opcode, 11b, _UART_, op1
	dd opcode
}

macro getg op1*{
	local opcode
	_mug_ins opcode, 0, _GPIO_, op1
	dd opcode
}

macro setg op1*{
	local opcode
	_mug_ins opcode, 1, _GPIO_, op1
	dd opcode
}

macro ing op1*{
	local opcode
	_mug_ins opcode, 10b, _GPIO_, op1
	dd opcode
}

macro outg op1*{
	local opcode
	_mug_ins opcode, 11b, _GPIO_, op1
	dd opcode
}

include "macros.inc"

_start_code:
; ��������������� ������ ��� �������� ����� � ascii-������
macro dword_to_uint arg{
	local temp_val, diver
	temp_val = arg
	diver = 1
	while diver*10 < arg
		diver = diver * 10
	end while
	while diver > 0
		db (temp_val / diver)+30h
		temp_val = temp_val mod diver
		diver = diver / 10
	end while
}

; �������� mif-����� � ����� ����������
postpone{
	local dword_now, _main, dwords_count
	_end_code:
	virtual at 0 as "mif"
		_main::
		db "-- Quartus Prime generated Memory Initialization File(.mif)", 0Ah
		db 0Ah
		db "WIDTH = 32;", 0Ah
		db "DEPTH = 4096;", 0Ah
		db 0Ah
		db "ADDRESS_RADIX = UNS;", 0Ah
		db "DATA_RADIX = UNS;", 0Ah
		db 0Ah
		db "CONTENT BEGIN", 0Ah
	end virtual
	dwords_count = ((_end_code-_start_code) shr 2)		;���������� ������� ����, ������ ���� ��������� � ������
	repeat dwords_count
		load dword_now dword from _start_code+(%-1)*4  	;��������� ������� dword �� ���������
		virtual _main									;���������� � mif
			db 09h										;��������� ��� �������
			dword_to_uint %-1							;����� dword`�
			db ": "
			dword_to_uint dword_now						;�������� � ���������� ����������� ascii
			db ";", 0Ah 								;���������� ������ � �������
		end virtual
	end repeat
	if dwords_count < 4096								;���� ������� ������ ���������, �� ��������� ������
		virtual _main									;� ���������� ���� ���������
			db 09h, "["
			dword_to_uint dwords_count
			db "..4096]:0;", 0Ah 
		end virtual
	end if
	virtual _main
		db "END;"
	end virtual
}