.386
.model flat, stdcall  
option casemap: none
includelib msvcrt.lib
printf proto c
scanf proto c


.data
$formatD  byte "%d ", 0
$formatC  byte "%c ", 0
$formatDNewLine  byte "%d", 0AH, 0
$formatCNewLine  byte "%c", 0AH, 0
$constA dword 10
$constB dword 20
$constC dword 50
$fibArr dword 10 dup(0)
$globalArr dword 20 dup(0)
$i dword 0
$n dword 0
$str0 byte "fib[0-9] are as follows:", 0AH, 0
$str1 byte "please input a number n", 0AH, 0
$str2 byte "n is too large, so it is set to 10", 0AH, 0
$str3 byte "please input an array of n ints", 0AH, 0


.code

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
mov ebx, dword ptr [ebp+8]
mov eax, dword ptr [$fibArr+ebx*4]
mov ecx, 0
cmp eax, ecx
jne L1
mov dword ptr [ebp+8], ebx
sub ebx, 1
push ebx
mov ecx, dword ptr [ebp+8]
mov dword ptr [$fibArr+ecx*4], eax
mov dword ptr [ebp+8], ecx
call fib
add esp, 4
mov ecx, dword ptr [ebp+8]
sub ecx, 2
push ecx
mov dword ptr [ebp-8], eax
mov dword ptr [ebp-12], ecx
call fib
add esp, 4
mov ecx, dword ptr [ebp-8]
add ecx, eax
mov edi, dword ptr [ebp+8]
mov dword ptr [$fibArr+edi*4], ecx
L1:
mov ebx, dword ptr [ebp+8]
mov eax, dword ptr [$fibArr+ebx*4]
jmp @fib_epilogue

; fib epilogue
@fib_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
fib endp

; sort
sort proc

; sort prologue
push ebp
mov ebp, esp
sub esp, 20
push esi
push edi
push ebx

; sort body
mov dword ptr [ebp-8], 0
L3:
mov eax, dword ptr [ebp-8]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L2
mov dword ptr [ebp-8], eax
add eax, 1
mov dword ptr [ebp-12], eax
L5:
mov eax, dword ptr [ebp-12]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L4
mov edi, dword ptr [ebp-8]
mov ecx, dword ptr [$globalArr+edi*4]
mov edx, dword ptr [$globalArr+eax*4]
cmp ecx, edx
jle L6
mov esi, dword ptr [$globalArr+edi*4]
mov dword ptr [ebp-16], esi
mov dword ptr [ebp-12], eax
mov dword ptr [$n], ebx
mov ebx, dword ptr [ebp-12]
mov eax, dword ptr [$globalArr+ebx*4]
mov dword ptr [$globalArr+edi*4], eax
mov ecx, dword ptr [ebp-16]
mov dword ptr [$globalArr+ebx*4], ecx
L6:
mov eax, dword ptr [ebp-12]
add eax, 1
mov dword ptr [ebp-12], eax
jmp L5
L4:
mov eax, dword ptr [ebp-8]
add eax, 1
mov dword ptr [ebp-8], eax
jmp L3
L2:

; sort epilogue
@sort_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
sort endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 32
push esi
push edi
push ebx

; main body
mov dword ptr [$fibArr+0*4], 1
mov dword ptr [$fibArr+1*4], 1
push 9
call fib
add esp, 4
mov dword ptr [ebp-24], 0
lea ebx, $str0
push ebx
mov dword ptr [ebp-8], eax
call printf
add esp, 4
L7:
mov ebx, dword ptr [ebp-24]
mov eax, dword ptr [$fibArr+ebx*4]
push eax
lea ecx, $formatDNewLine
push ecx
mov dword ptr [$fibArr+ebx*4], eax
call printf
add esp, 8
mov dword ptr [ebp-24], ebx
add ebx, 1
mov dword ptr [ebp-24], ebx
mov eax, dword ptr [ebp-24]
mov ecx, 10
cmp eax, ecx
jl L7
lea ecx, $str1
push ecx
mov dword ptr [ebp-24], eax
call printf
add esp, 4
lea eax, dword ptr [$n]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov eax, dword ptr [$n]
mov ecx, 10
cmp eax, ecx
jle L8
mov dword ptr [$n], 10
lea eax, $str2
push eax
call printf
add esp, 4
L8:
lea eax, $str3
push eax
call printf
add esp, 4
mov dword ptr [ebp-24], 0
L10:
mov eax, dword ptr [ebp-24]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L9
lea ecx, dword ptr [ebp-28]
push ecx
lea ecx, [$formatD]
push ecx
mov dword ptr [ebp-24], eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-24]
mov ecx, dword ptr [ebp-28]
mov dword ptr [$globalArr+eax*4], ecx
add eax, 1
mov dword ptr [ebp-24], eax
jmp L10
L9:
call sort
add esp, 0
mov dword ptr [ebp-24], 0
L12:
mov eax, dword ptr [ebp-24]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L11
mov ecx, dword ptr [$globalArr+eax*4]
push ecx
lea edi, $formatDNewLine
push edi
mov dword ptr [ebp-24], eax
mov eax, dword ptr [ebp-24]
mov dword ptr [$globalArr+eax*4], ecx
call printf
add esp, 8
mov eax, dword ptr [ebp-24]
add eax, 1
mov dword ptr [ebp-24], eax
jmp L12
L11:
mov eax, 20
cdq
mov ebx, 10
idiv ebx
add eax, 50
mov ebx, dword ptr [ebp-20]
mov ebx, -40
mov dword ptr [ebp-16], eax
mov eax, 4
cdq
mov ecx, 3
idiv ecx
imul ebx, eax
add ebx, 432
sub ebx, 5
add ebx, 3
mov ecx, dword ptr [ebp-16]
sub ecx, ebx
mov dword ptr [$n], ecx
mov edi, dword ptr [$n]
push edi
lea edx, $formatDNewLine
push edx
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-16], ecx
call printf
add esp, 8

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
