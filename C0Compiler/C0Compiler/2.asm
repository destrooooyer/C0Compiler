.386
.model flat, stdcall  
option casemap: none
includelib msvcrt.lib
printf proto c
scanf proto c


.data
$formatD  byte "%d", 0
$_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ dword 0
$a dword 12 dup(0)
$b dword 0
$c dword 10 dup(0)
$const_a dword 1
$const_b dword -210
$const_c dword 43
$const_d dword 42
$const_e dword 97
$const_f dword 49
$str0 byte "123", 0
$str1 byte "123", 0


.code

; funa
funa proc

; funa prologue
push ebp
mov ebp, esp
sub esp, 28
push esi
push edi
push ebx

; funa body
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp+12]
cmp eax, ebx
jne L1
mov dword ptr [ebp+8], ebx
jmp L2
L1:
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp+12]
cmp eax, ebx
jle L3
cmp eax, ebx
jl L4
L4:
L3:
L2:
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp+12]
cmp eax, ebx
jge L5
cmp eax, ebx
jg L6
cmp eax, ebx
je L7
mov ecx, 2
mov ecx, 0
cmp ecx, ecx
je L8
L8:
L7:
L6:
L5:
L9:
mov eax, dword ptr [ebp+12]
mov dword ptr [ebp+8], eax
mov ebx, dword ptr [ebp+8]
cmp ebx, eax
jne L9
mov dword ptr [ebp-24], 0
L11:
mov eax, dword ptr [ebp-24]
mov ebx, 10
cmp eax, ebx
jge L10
add eax, 1
mov dword ptr [ebp-24], eax
jmp L11
L10:
push dword ptr [ebp+12]
push dword ptr [ebp+8]
call funa
add esp, 8
mov dword ptr [ebp-4], eax
call func
add esp, 8
mov eax, dword ptr [ebp+8]
mov dword ptr [ebp+8], eax
mov eax, dword ptr [ebp+8]
mov ebx, 0
sub eax, ebx
add eax, eax
mov dword ptr [ebp+8], eax
mov eax, dword ptr [$a+1*4]
mov ebx, dword ptr [ebp+8]
imul eax, ebx
mov dword ptr [ebp-12], eax
mov eax, dword ptr [ebp-12]
cdq
idiv ebx
add eax, 1
push dword ptr [ebp+12]
push ebx
mov dword ptr [ebp-12], eax
call funa
add esp, 8
mov ecx, dword ptr [ebp-12]
add ecx, eax
add ecx, ebx
add ecx, 50
mov dword ptr [ebp+8], ecx
mov eax, 1
lea ebx, dword ptr [ebp+8]
push ebx
lea ebx, [$formatD]
push ebx
mov dword ptr [ebp-16], eax
mov dword ptr [ebp-12], ecx
call scanf
add esp, 8
lea eax, dword ptr [ebp+12]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, $str0
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp+8]
push eax
lea ebx, $formatD
push ebx
mov dword ptr [ebp+8], eax
call printf
add esp, 8
lea eax, $str1
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp+8]
push eax
lea ebx, $formatD
push ebx
mov dword ptr [ebp+8], eax
call printf
add esp, 8

; funa epilogue
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
funa endp

; funb
funb proc

; funb prologue
push ebp
mov ebp, esp
push esi
push edi
push ebx

; funb body

; funb epilogue
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
funb endp

; func
func proc

; func prologue
push ebp
mov ebp, esp
push esi
push edi
push ebx

; func body

; func epilogue
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
func endp

; fib
fib proc

; fib prologue
push ebp
mov ebp, esp
sub esp, 20
push esi
push edi
push ebx

; fib body
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L12
mov eax, 1
jmp L13
L12:
mov eax, dword ptr [ebp+8]
mov ebx, 2
cmp eax, ebx
jne L14
mov eax, 1
jmp L15
L14:
mov eax, dword ptr [ebp+8]
mov ebx, 1
sub eax, ebx
push eax
mov dword ptr [ebp-4], eax
call fib
add esp, 4
mov ebx, dword ptr [ebp+8]
mov ecx, 2
sub ebx, ecx
push ebx
mov dword ptr [ebp-8], eax
call fib
add esp, 4
mov ecx, dword ptr [ebp-8]
add ecx, eax
mov eax, ecx
L15:
L13:

; fib epilogue
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
fib endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 36
push esi
push edi
push ebx

; main body
mov dword ptr [ebp-32], 0
mov dword ptr [ebp-16], 1
L17:
mov eax, dword ptr [ebp-16]
mov ebx, 10
cmp eax, ebx
jge L16
mov dword ptr [$c+eax*4], 0
add eax, 1
mov dword ptr [ebp-16], eax
jmp L17
L16:
lea eax, dword ptr [ebp-20]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-24]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-28]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-20]
mov ebx, 5
cmp eax, ebx
jge L18
mov dword ptr [$c+9*4], eax
L18:
mov eax, dword ptr [ebp-12]
mov eax, 12
mov dword ptr [ebp-12], eax
mov eax, dword ptr [ebp-12]
cdq
mov ebx, 12
idiv ebx
imul eax, 3
add eax, 6
mov ebx, dword ptr [$c+eax*4]
mov dword ptr [ebp-32], ebx
mov ecx, dword ptr [ebp-32]
push ecx
lea edi, $formatD
push edi
mov dword ptr [ebp-12], eax
mov dword ptr [ebp-32], ecx
call printf
add esp, 8
mov eax, dword ptr [ebp-24]
mov ecx, dword ptr [ebp-28]
imul eax, ecx
mov edi, dword ptr [ebp-32]
add eax, edi
mov dword ptr [ebp-32], eax
mov edi, dword ptr [ebp-32]
push edi
lea edx, $formatD
push edx
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-28], ecx
call printf
add esp, 8
push dword ptr [ebp-20]
call fib
add esp, 4
add eax, edi
mov dword ptr [ebp-32], eax
mov ecx, dword ptr [ebp-32]
push ecx
lea edi, $formatD
push edi
mov dword ptr [ebp-8], eax
mov dword ptr [ebp-32], ecx
call printf
add esp, 8

; main epilogue
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
main endp
