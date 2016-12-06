.386
.model flat, stdcall  
option casemap: none
includelib msvcrt.lib
printf proto c
scanf proto c


.data
$formatD  byte "%d", 0
$formatC  byte "%c", 0
$formatDNewLine  byte "%d", 0AH, 0
$formatCNewLine  byte "%c", 0AH, 0
$A2 dword 1 dup(0)
$A3 dword 20 dup(0)
$A4 dword 0
$_ dword 0
$_9 dword 0
$_A dword 0
$__ dword 0
$_a dword 0
$a1 dword 0
$a2 dword 0
$a3 dword 10 dup(0)
$a4 dword 1 dup(0)
$a5 dword 0
$cChar0 dword 95
$cChar1 dword 97
$cChar2 dword 65
$cChar3 dword 43
$cChar4 dword 42
$cChar5 dword 48
$cInt1 dword 10
$cInt2 dword -1
$cInt3 dword 0
$cMaxInt dword 2147483647
$cMinInt dword -2147483647
$cmaxint dword 2147483647
$str0 byte " !#$%&'()*+,-./0123456789:;<=> @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~", 0AH, 0
$str1 byte " ", 0AH, 0
$str10 byte "if ok....perhaps", 0AH, 0
$str11 byte "_a is", 0AH, 0
$str12 byte "result:0", 0AH, 0
$str13 byte "result:", 0AH, 0
$str14 byte "result:", 0AH, 0
$str15 byte "result:", 0AH, 0
$str16 byte " ", 0AH, 0
$str2 byte "helloworld!", 0AH, 0
$str3 byte "input a char and enter:", 0AH, 0
$str4 byte "please input number counts:", 0AH, 0
$str5 byte "_a is", 0AH, 0
$str6 byte "_a is", 0AH, 0
$str7 byte "_a is", 0AH, 0
$str8 byte "_a is", 0AH, 0
$str9 byte "_a is", 0AH, 0


.code

; PrintHelloWorld
@PrintHelloWorld proc

; PrintHelloWorld prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; PrintHelloWorld body
mov dword ptr [ebp-4], 233
lea eax, $str0
push eax
call printf
add esp, 4
lea eax, $str1
push eax
call printf
add esp, 4
lea eax, $str2
push eax
call printf
add esp, 4
mov eax, 42
push eax
lea ebx, $formatCNewLine
push ebx
call printf
add esp, 8
mov eax, 95
push eax
lea ebx, $formatCNewLine
push ebx
call printf
add esp, 8
mov eax, 10
push eax
lea ebx, $formatDNewLine
push ebx
call printf
add esp, 8
mov eax, dword ptr [ebp-4]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-4], eax
call printf
add esp, 8
jmp @PrintHelloWorld_epilogue

; PrintHelloWorld epilogue
@PrintHelloWorld_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@PrintHelloWorld endp

; SquareSum
@SquareSum proc

; SquareSum prologue
push ebp
mov ebp, esp
sub esp, 48
push esi
push edi
push ebx

; SquareSum body
mov eax, dword ptr [ebp+8]
imul eax, -1
mov dword ptr [ebp-4], eax
neg eax
add eax, 0
mov dword ptr [ebp-40], eax
mov ebx, dword ptr [ebp+8]
imul ebx, 0
mov dword ptr [ebp-40], ebx
mov ecx, dword ptr [ebp+8]
imul ecx, 0
mov dword ptr [ebp-40], ecx
mov edi, dword ptr [ebp+8]
imul edi, edi
mov dword ptr [ebp-40], edi
mov edx, dword ptr [ebp+8]
imul edx, edx
mov dword ptr [ebp-24], edx
neg edx
add edx, 0
mov dword ptr [ebp-28], edx
neg edx
add edx, 0
mov esi, dword ptr [ebp+12]
imul esi, esi
add edx, esi
mov dword ptr [ebp-40], edx
mov dword ptr [ebp-8], eax
mov eax, dword ptr [ebp-40]
jmp @SquareSum_epilogue

; SquareSum epilogue
@SquareSum_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@SquareSum endp

; _myStop
@_myStop proc

; _myStop prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; _myStop body
lea eax, $str3
push eax
call printf
add esp, 4
lea eax, dword ptr [ebp-4]
push eax
lea eax, [$formatC]
push eax
call scanf
add esp, 8

; _myStop epilogue
@_myStop_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@_myStop endp

; NsquareSum
@NsquareSum proc

; NsquareSum prologue
push ebp
mov ebp, esp
sub esp, 56
push esi
push edi
push ebx

; NsquareSum body
mov eax, dword ptr [ebp+8]
mov dword ptr [ebp-12], eax
mov ebx, dword ptr [ebp+12]
mov ecx, 1
cmp ebx, ecx
jne L1
jmp @NsquareSum_epilogue
L1:
mov eax, dword ptr [ebp+12]
sub eax, 1
push eax
push dword ptr [ebp+8]
mov dword ptr [ebp-4], eax
call @NsquareSum
add esp, 8
mov ebx, dword ptr [ebp-12]
imul eax, ebx
mov dword ptr [ebp-12], eax
mov ebx, dword ptr [ebp-12]
mov eax, ebx
jmp @NsquareSum_epilogue

; NsquareSum epilogue
@NsquareSum_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@NsquareSum endp

; NextAlphabet
@NextAlphabet proc

; NextAlphabet prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; NextAlphabet body
mov eax, dword ptr [ebp+8]
add eax, 1
jmp @NextAlphabet_epilogue

; NextAlphabet epilogue
@NextAlphabet_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@NextAlphabet endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 104
push esi
push edi
push ebx

; main body
call @PrintHelloWorld
add esp, 0
lea eax, $str4
push eax
call printf
add esp, 4
lea eax, dword ptr [ebp-92]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov dword ptr [ebp-76], 0
mov dword ptr [$_a], 666
mov eax, dword ptr [ebp-4]
mov eax, -20
mov dword ptr [ebp-80+0*4], eax
mov ebx, dword ptr [ebp-80+0*4]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-80+0*4], ebx
call printf
add esp, 8
mov dword ptr [ebp-72+0*4], 97
mov dword ptr [ebp-72+1*4], 98
mov dword ptr [ebp-72+2*4], 99
push dword ptr [ebp-72+0*4]
call @NextAlphabet
add esp, 4
push eax
lea ebx, $formatCNewLine
push ebx
mov dword ptr [ebp-8], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, dword ptr [ebp-80+0*4]
cmp eax, ebx
jle L2
mov ebx, dword ptr [ebp-80+0*4]
sub ebx, 1
mov dword ptr [$_a], ebx
L2:
lea eax, $str5
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, dword ptr [ebp-80+0*4]
cmp eax, ebx
jge L3
mov dword ptr [$_a], 2
L3:
lea eax, $str6
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, 2
cmp eax, ebx
jne L4
mov ebx, dword ptr [ebp-80+0*4]
imul ebx, 2
mov dword ptr [$_a], ebx
L4:
lea eax, $str7
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, dword ptr [ebp-80+0*4]
cmp eax, ebx
je L5
mov dword ptr [$_a], 4
L5:
lea eax, $str8
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, 4
cmp eax, ebx
jl L6
mov ebx, dword ptr [ebp-20]
mov ebx, 0
mov dword ptr [$_a], ebx
L6:
lea eax, $str9
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [$_a]
mov ebx, 0
cmp eax, ebx
jg L7
lea ebx, $str10
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov dword ptr [$_a], 666
L7:
lea eax, $str11
push eax
call printf
add esp, 4
mov eax, dword ptr [$_a]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$_a], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-92]
mov ebx, 0
cmp eax, ebx
jne L8
lea ebx, $str12
push ebx
mov dword ptr [ebp-92], eax
call printf
add esp, 4
jmp L9
L8:
mov eax, dword ptr [ebp-92]
mov ebx, 1
cmp eax, ebx
jne L10
lea ebx, dword ptr [ebp-96]
push ebx
lea ebx, [$formatD]
push ebx
mov dword ptr [ebp-92], eax
call scanf
add esp, 8
lea eax, $str13
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp-96]
add eax, 10
sub eax, 10
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-24], eax
call printf
add esp, 8
jmp L11
L10:
mov eax, dword ptr [ebp-92]
mov ebx, 2
cmp eax, ebx
jne L12
lea ebx, dword ptr [ebp-96]
push ebx
lea ebx, [$formatD]
push ebx
mov dword ptr [ebp-92], eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-100]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, $str14
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp-100]
sub eax, 2
add eax, 2
push eax
mov ebx, dword ptr [ebp-96]
add ebx, 0
push ebx
mov dword ptr [ebp-32], eax
mov dword ptr [ebp-28], ebx
call @SquareSum
add esp, 8
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-36], eax
call printf
add esp, 8
jmp L13
L12:
mov eax, dword ptr [ebp-92]
mov ebx, 0
cmp eax, ebx
jle L14
mov dword ptr [ebp-100], 0
mov ebx, dword ptr [ebp-76]
cmp ebx, eax
jge L15
L16:
lea eax, dword ptr [ebp-96]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
push dword ptr [ebp-92]
push dword ptr [ebp-96]
call @NsquareSum
add esp, 8
mov ebx, dword ptr [ebp-100]
add eax, ebx
mov dword ptr [ebp-100], eax
mov ebx, dword ptr [ebp-76]
add ebx, 1
mov dword ptr [ebp-76], ebx
mov ecx, dword ptr [ebp-76]
mov edi, dword ptr [ebp-92]
cmp ecx, edi
jl L16
L15:
mov eax, dword ptr [ebp-100]
mov dword ptr [$a3+0*4], eax
lea ebx, $str15
push ebx
mov dword ptr [ebp-100], eax
call printf
add esp, 4
mov eax, dword ptr [$a3+0*4]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [$a3+0*4], eax
call printf
add esp, 8
L14:
L13:
L11:
L9:
call @_myStop
add esp, 0
push 1
push 1
call @NsquareSum
add esp, 8
mov dword ptr [ebp-76], eax
lea ebx, $str16
push ebx
mov dword ptr [ebp-48], eax
call printf
add esp, 4
mov eax, dword ptr [ebp-96]
mov ebx, dword ptr [ebp-100]
add eax, ebx
mov dword ptr [ebp-88], eax
mov ecx, dword ptr [ebp-96]
add ecx, ebx
mov dword ptr [ebp-76], ecx
mov edi, dword ptr [ebp-96]
mov dword ptr [ebp-84], edi
mov edx, dword ptr [ebp-84]
add edx, ebx
mov dword ptr [ebp-84], edx
mov dword ptr [ebp-52], eax
mov dword ptr [ebp-100], ebx
mov dword ptr [ebp-56], ecx
call @_myStop
add esp, 0
jmp @main_epilogue

; main epilogue
@main_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
main endp
end main
