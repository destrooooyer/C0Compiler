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
$a dword 10 dup(0)
$c dword 0
$cona dword 1
$conb dword 2
$conc dword 3
$cong dword 4
$conh dword 5
$coni dword 6
$conj dword 7
$str0 byte "OVE ", 0AH, 0
$str1 byte " TO ", 0AH, 0
$str10 byte "ERROR", 0AH, 0
$str2 byte " ", 0AH, 0
$str3 byte "RETURN ERROR", 0AH, 0
$str4 byte "NO", 0AH, 0
$str5 byte ":", 0AH, 0
$str6 byte " ", 0AH, 0
$str7 byte " ", 0AH, 0
$str8 byte "IFCORRECT", 0AH, 0
$str9 byte ">=", 0AH, 0


.code

; emptyfunc
@emptyfunc proc

; emptyfunc prologue
push ebp
mov ebp, esp
push esi
push edi
push ebx

; emptyfunc body

; emptyfunc epilogue
@emptyfunc_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@emptyfunc endp

; emptyfunc2
@emptyfunc2 proc

; emptyfunc2 prologue
push ebp
mov ebp, esp
push esi
push edi
push ebx

; emptyfunc2 body

; emptyfunc2 epilogue
@emptyfunc2_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@emptyfunc2 endp

; testchar
@testchar proc

; testchar prologue
push ebp
mov ebp, esp
push esi
push edi
push ebx

; testchar body
mov eax, 48
jmp @testchar_epilogue

; testchar epilogue
@testchar_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@testchar endp

; fibonacci
@fibonacci proc

; fibonacci prologue
push ebp
mov ebp, esp
sub esp, 28
push esi
push edi
push ebx

; fibonacci body
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L1
mov eax, 1
jmp @fibonacci_epilogue
L1:
mov eax, dword ptr [ebp+8]
mov ebx, 2
cmp eax, ebx
je L2
mov dword ptr [ebp+8], eax
add eax, -1
push eax
mov dword ptr [ebp-4], eax
call @fibonacci
add esp, 4
mov ebx, dword ptr [ebp+8]
sub ebx, 2
push ebx
mov dword ptr [ebp-8], eax
mov dword ptr [ebp-12], ebx
call @fibonacci
add esp, 4
mov ebx, dword ptr [ebp-8]
add ebx, eax
add ebx, 0
mov eax, ebx
jmp @fibonacci_epilogue
L2:
mov eax, 1
jmp @fibonacci_epilogue

; fibonacci epilogue
@fibonacci_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@fibonacci endp

; move
@move proc

; move prologue
push ebp
mov ebp, esp
sub esp, 12
push esi
push edi
push ebx

; move body
mov eax, 77
push eax
lea ebx, $formatCNewLine
push ebx
call printf
add esp, 8
lea eax, $str0
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp+8]
push eax
lea ebx, $formatCNewLine
push ebx
mov dword ptr [ebp+8], eax
call printf
add esp, 8
lea eax, $str1
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp+12]
push eax
lea ebx, $formatCNewLine
push ebx
mov dword ptr [ebp+12], eax
call printf
add esp, 8
lea eax, $str2
push eax
call printf
add esp, 4

; move epilogue
@move_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@move endp

; hanoi
@hanoi proc

; hanoi prologue
push ebp
mov ebp, esp
sub esp, 12
push esi
push edi
push ebx

; hanoi body
mov eax, 2
mov ebx, dword ptr [ebp+8]
cmp eax, ebx
jle L3
push dword ptr [ebp+20]
push dword ptr [ebp+12]
mov dword ptr [ebp+8], ebx
call @move
add esp, 8
jmp @hanoi_epilogue
lea eax, $str3
push eax
call printf
add esp, 4
L3:
push dword ptr [ebp+16]
push dword ptr [ebp+20]
push dword ptr [ebp+12]
mov eax, dword ptr [ebp+8]
sub eax, 1
push eax
mov dword ptr [ebp-4], eax
call @hanoi
add esp, 16
push dword ptr [ebp+20]
push dword ptr [ebp+12]
call @move
add esp, 8
push dword ptr [ebp+20]
push dword ptr [ebp+12]
push dword ptr [ebp+16]
mov eax, dword ptr [ebp+8]
sub eax, 1
push eax
mov dword ptr [ebp-8], eax
call @hanoi
add esp, 16

; hanoi epilogue
@hanoi_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@hanoi endp

; getpower
@getpower proc

; getpower prologue
push ebp
mov ebp, esp
sub esp, 32
push esi
push edi
push ebx

; getpower body
mov eax, dword ptr [ebp+12]
mov ebx, 2
cmp eax, ebx
jl L4
mov dword ptr [ebp+12], eax
sub eax, 1
push eax
push dword ptr [ebp+8]
mov dword ptr [ebp-16], eax
call @getpower
add esp, 8
mov dword ptr [ebp-28], eax
mov ebx, dword ptr [ebp+12]
sub ebx, 1
mov ecx, dword ptr [ebp-28]
mov edi, dword ptr [ebp+8]
imul ecx, edi
mov dword ptr [$a+ebx*4], ecx
mov dword ptr [ebp+8], edi
mov edx, dword ptr [ebp-28]
imul edi, edx
mov eax, edi
jmp @getpower_epilogue
L4:
mov eax, 1
mov ebx, dword ptr [ebp+12]
cmp eax, ebx
jg L5
mov dword ptr [ebp+12], ebx
sub ebx, 1
mov eax, dword ptr [ebp+8]
mov dword ptr [$a+ebx*4], eax
jmp @getpower_epilogue
L5:

; getpower epilogue
@getpower_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@getpower endp

; getsqr
@getsqr proc

; getsqr prologue
push ebp
mov ebp, esp
sub esp, 40
push esi
push edi
push ebx

; getsqr body
mov dword ptr [ebp-36], 1
mov eax, dword ptr [ebp+8]
cdq
mov ebx, dword ptr [ebp-36]
idiv ebx
cmp ebx, eax
jge L6
L7:
mov eax, dword ptr [ebp-36]
add eax, 1
mov dword ptr [ebp-36], eax
mov dword ptr [ebp-8], eax
mov eax, dword ptr [ebp+8]
cdq
mov ebx, dword ptr [ebp-36]
idiv ebx
cmp ebx, eax
jl L7
L6:
mov eax, dword ptr [ebp+8]
neg eax
add eax, 0
mov ebx, dword ptr [ebp-36]
imul ebx, ebx
add eax, ebx
mov ecx, dword ptr [ebp-24]
mov ecx, -1
mov edi, dword ptr [ebp-36]
add ecx, edi
mov dword ptr [ebp-36], edi
sub edi, 1
imul ecx, edi
mov edx, dword ptr [ebp+8]
neg ecx
add ecx, edx
cmp eax, ecx
jge L8
mov esi, dword ptr [ebp-36]
mov eax, esi
jmp @getsqr_epilogue
L8:
mov eax, dword ptr [ebp-36]
sub eax, 1
jmp @getsqr_epilogue

; getsqr epilogue
@getsqr_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@getsqr endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 596
push esi
push edi
push ebx

; main body
mov dword ptr [ebp-56], 1234567891
mov dword ptr [ebp-52], 65
mov dword ptr [ebp-524+0*4], 5
mov eax, dword ptr [ebp-524+0*4]
mov dword ptr [ebp-524+eax*4], 1
mov dword ptr [ebp-524+43*4], 2
lea eax, dword ptr [ebp-60]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-120]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-60]
mov ebx, 1
cmp eax, ebx
jne L9
push 10
push dword ptr [ebp-120]
mov dword ptr [ebp-60], eax
call @getpower
add esp, 8
mov dword ptr [ebp-116], 0
mov ebx, dword ptr [ebp-116]
mov ecx, 10
cmp ebx, ecx
jge L10
L11:
lea eax, $str4
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp-116]
add eax, 1
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-8], eax
call printf
add esp, 8
lea eax, $str5
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp-116]
mov eax, dword ptr [$a+eax*4]
push eax
lea ebx, $formatDNewLine
push ebx
mov ebx, dword ptr [ebp-116]
mov dword ptr [$a+ebx*4], eax
mov dword ptr [ebp-116], ebx
call printf
add esp, 8
lea eax, $str6
push eax
call printf
add esp, 4
lea eax, $str7
push eax
call printf
add esp, 4
mov eax, 1
mov ebx, 0
cmp eax, ebx
je L12
mov eax, dword ptr [ebp-116]
add eax, 1
mov dword ptr [ebp-116], eax
L12:
mov eax, dword ptr [ebp-116]
mov ebx, 10
cmp eax, ebx
jl L11
L10:
L9:
mov eax, dword ptr [ebp-60]
mov ebx, 2
cmp eax, ebx
jne L13
push 67
push 66
push 65
push dword ptr [ebp-120]
mov dword ptr [ebp-60], eax
call @hanoi
add esp, 16
jmp L14
L13:
mov eax, dword ptr [ebp-60]
mov ebx, 3
cmp eax, ebx
jne L15
push dword ptr [ebp-120]
mov dword ptr [ebp-60], eax
call @fibonacci
add esp, 4
mov dword ptr [ebp-112], eax
mov ebx, dword ptr [ebp-112]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-16], eax
mov dword ptr [ebp-112], ebx
call printf
add esp, 8
L15:
L14:
mov eax, dword ptr [ebp-60]
mov ebx, 4
cmp eax, ebx
jne L16
push dword ptr [ebp-120]
mov dword ptr [ebp-60], eax
call @getsqr
add esp, 4
mov dword ptr [ebp-124], eax
mov ebx, dword ptr [ebp-124]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-20], eax
mov dword ptr [ebp-124], ebx
call printf
add esp, 8
jmp L17
L16:
mov eax, dword ptr [ebp-60]
mov ebx, 5
cmp eax, ebx
jne L18
mov dword ptr [ebp-60], eax
call @testchar
add esp, 0
push eax
lea ebx, $formatCNewLine
push ebx
mov dword ptr [ebp-24], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-28]
mov eax, 6
mov dword ptr [ebp-28], eax
mov eax, dword ptr [ebp-28]
cdq
mov ebx, 6
idiv ebx
mov ebx, dword ptr [ebp-524+0*4]
imul eax, ebx
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-28], eax
call printf
add esp, 8
jmp L19
L18:
mov eax, dword ptr [ebp-60]
mov ebx, 6
cmp eax, ebx
jne L20
mov ebx, dword ptr [ebp-524+0*4]
mov ecx, 0
cmp ebx, ecx
je L21
lea ebx, $str8
push ebx
mov dword ptr [ebp-60], eax
call printf
add esp, 4
L21:
mov eax, dword ptr [ebp-524+0*4]
mov ebx, 0
cmp eax, ebx
je L22
L23:
mov eax, dword ptr [ebp-524+0*4]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-524+0*4], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-524+0*4]
sub eax, 1
mov dword ptr [ebp-524+0*4], eax
mov ebx, dword ptr [ebp-524+0*4]
mov ecx, 0
cmp ebx, ecx
jne L23
L22:
jmp L24
L20:
mov eax, dword ptr [ebp-36]
mov eax, -7
mov ebx, dword ptr [ebp-60]
cmp ebx, eax
jne L25
lea ecx, dword ptr [ebp-528]
push ecx
lea ecx, [$formatD]
push ecx
mov dword ptr [ebp-36], eax
mov dword ptr [ebp-60], ebx
call scanf
add esp, 8
lea eax, dword ptr [ebp-532]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-536]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-540]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-544]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-548]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-528]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-528], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-532]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-532], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-536]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-536], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-540]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-540], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-544]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-544], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-548]
push eax
lea ebx, $formatDNewLine
push ebx
mov dword ptr [ebp-548], eax
call printf
add esp, 8
jmp L26
L25:
mov eax, dword ptr [ebp-60]
mov ebx, 0
cmp eax, ebx
jne L27
lea ebx, dword ptr [ebp-64]
push ebx
lea ebx, [$formatC]
push ebx
mov dword ptr [ebp-60], eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-64]
mov ebx, 97
cmp eax, ebx
jne L28
mov ebx, 1
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-64], eax
call printf
add esp, 8
jmp L29
L28:
mov eax, dword ptr [ebp-64]
mov ebx, 98
cmp eax, ebx
jne L30
mov ebx, 2
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-64], eax
call printf
add esp, 8
jmp L31
L30:
lea eax, $str9
push eax
call printf
add esp, 4
mov eax, 3
push eax
lea ebx, $formatDNewLine
push ebx
call printf
add esp, 8
L31:
L29:
jmp L32
L27:
lea eax, $str10
push eax
call printf
add esp, 4
L32:
L26:
L24:
L19:
L17:
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
