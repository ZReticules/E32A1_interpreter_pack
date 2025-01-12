!Примечание!
Для компиляции интерпретатора самостоятельно папка TOOLS должна содержать в себе файлы из <https://github.com/ZReticules/FASM_OOP/tree/multiarch>  
Основной репозиторий проекта <https://github.com/Teimir/UARTP>

Описание синтаксиса инструкций E32A1 Fasm  
1. Общие сведения:  
Данный транслятор представляет все инструкции "как есть", без каких-либо оберток.  
Однако так же доступны макросы, облегчающие написание кода.  
При создании сохранялись основные признаки синтаксиса FASM.  
Инструкции имеют от 0 до 4 операндов.  
Регистр 31 является указателем инструкций.  
Список доступных инструкций:  

| Инструкция| описание| Кол-во операндов |  
| ---| ---| ---| 
| nop| пустая инструкция|  0 оп |   
| hlt| конец программы|  0 оп |   
| setu| задать скорость UART|  1 оп |   
| getu| получить инфорацию UART|  1 оп |   
| inu| прочитать с UART|  1 оп |   
| outu| отправить на UART|  1 оп |   
| setg| установить параметры GPIO|  1 оп |   
| getg| получить параметры GPIO|  1 оп |   
| ing| прочитать с GPIO|  1 оп |   
| outg| отправить на GPIO|  1 оп |   
| inm| прочитать с памяти|  2 оп |   
| outm| записать в память|  2 оп |   
| mov| копирование данных|  2 оп |   
| not| логическое "не"|  2 оп |   
| add| сложение|  3 оп |   
| sub| вычитание|  3 оп |   
| and| логическое "и"|  3 оп |   
| or| логическое "или"|  3 оп |   
| xor| исключающее "или"|  3 оп |   
| shr| логический сдвиг вправо|  3 оп |   
| shl| логический сдвиг влево|  3 оп |   
| sar| арифметический сдвиг вправо|  3 оп |   
| sal| арифметический сдвиг влево|  3 оп |   
| shrd| сдвиг вправо двойной точности|  4 оп |   
| shld| сдвиг влево двойной точности|  4 оп |  
| ror| циклический сдвиг вправо|  3 оп |   
| rol| циклический сдвиг влево|  3 оп |   
| cmpa| op1 = op2 > op3 ? -1 : 0|  3 оп |   
| cmpe| op1 = op2 == op3 ? -1 : 0|  3 оп |   
| cmpb| op1 = op2 < op3 ? -1 : 0|  3 оп |   
| cmovnz| op1 = op2 == -1 ? op3 : op1|  3 оп |   
| cmovz| op1 = op2 == 0 ? op3 : op1|  3 оп |  
  
Все инструкции имеют следующий общий синтаксис:  
instr op1, op2, op3, op4  
Где:  
1.1. op1 - назначение, может быть только регистром (r:n)  
1.2. op2 - может быть регистром или литералом, если op2 - литерал,  
	то op3 не существует. Тогда op2 - длинный литерал (20 бит).  
	Для инструкций работы с памятью op2 может быть только регистром.  
1.3. op3 - существует только если op2 регистр. Можеть быть регистром  
	или коротким литералом (15 бит).   
1.4. op4 - существует только если op3 регистр. Может быть только регистром.
Количество операндов для каждой инструкции см. список инструкций.  
  
2. Встроенные макросы
  
Некоторые макросы могут затирать или изменять значения старших регистров. При использовании макросов считать, что старшие 7 регистров(от r:30 и ниже) могут быть затерты. r:30 считается регистром программного стека.  
  
2.1. lds  
Для удобства работы с памятью был добавлен макрос lds. Синтаксис:  
2.1.1. Загрузка в память по адресу в регистре  
	`lds [r:n], r:n`  
2.1.2. Загрузка из памяти по адресу в регистре  
	`lds r:n, [r:n]`  
2.2. entry  
Макрос entry указывает точку входа в программу. Если точка входа указана, то в начале программы выполяется инициализация регистра стека(r:30) и выполняется переход в указанную точку. Использование возможно в любой точке программы, но только один раз.  
2.3. `_push и _pop`  
Эти макросы программного стека, работают аналогично push и pop в x86. ВВозможными операндами для первого макроса являются регистры, константы и память(например, `_push [r:0]` сохранит в стек значение по адресу r:0). Ссылки на память только регистровые.  
2.4. ccall  
Этот макрос предназначен для вызова функций. Первый его аргумент - адрес точки назначения. Может быть регистром, регистровой ссылкой на память([r:0]), абсолютной ссылкой на память ([0], [label addr]) или непосредственным значением метки(addr добавляется автоматически, указывать при вызове не нужно). Остальныx аргументов переменное количество, они имеют те же ограничения(но addr автоматически к константам уже не добавляется), их значения записываются в регистры r:1-r:n, где n - количество аргументов. Пример:  
`ccall print, c_str addr`  
Вызовет функцию print и передаст в r:1 адрес метки c_str.  
Макрос сохраняет в стек адрес возврата, поэтому для выхода из функции можно использовать команду `lds r:31, [r:30]`. ccall сам затирает стек после возврата.  
  
3. Формат программы  
Программа имеет следующий формат:  
```asm  
	include "ISA.inc"		;полный путь к ISA.inc, если программа в другой папке  
	...						;инструкции  
```	  
Для компиляции и получения файла формата .mif используется fasm.exe.  
Ему в качестве аргумента командной строки передается адрес файла с программой.  
В папке с программой он создаст файлы формата .bin с инструкциями в двоичном  
виде и файл формата .mif для Quartus.  
Так же в пакет входит интерпретатор, который может исполнять двоичный выходной файл. Использование:  
interpreter.exe filename.bin  
В качесте эмуляции работы uart используется консоль. Инструкции gpio не поддерживаются.  
  
4. Стандартные заголовочные файлы.  
4.1. uio.inc  
Содержит 2 функции, включаемые в исходник при использовании.  
4.1.1. print    
Вывод на uart asciiz строку по адресу из r:1.  
4.1.2. input  
Блокирующий ввод! Получает с uart строку длиной не более r:2 и записывает ее в память по адресу в r:1. Назначение должно иметь достаточно памяти, чтобы вместить завершающий 0 после получения всех символов. Символ возврата каретки(0xD) считается завершением ввода и не добавляется к результирующей строке. В r:0 возвращается количество считанных символов.  
