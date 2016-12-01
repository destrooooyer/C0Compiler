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
$constA dword 10
$constB dword 20
$constC dword 50
$fibArr dword 10 dup(0)
$globalArr dword 20 dup(0)
$i dword 0
$n dword 0
$str0 byte "input a char", 0AH, 0
$str1 byte "fib[0-9] are as follows:", 0AH, 0
$str2 byte "input a number n", 0AH, 0
$str3 byte "n is too large, so it is set to 10", 0AH, 0
$str4 byte "input an array of n ints", 0AH, 0
$str5 byte "the following number should be -338", 0AH, 0
$str6 byte "input a number n", 0AH, 0
$str7 byte "the prime numbers that are less than n are as follows", 0AH, 0


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
mov eax, dword ptr [ebp+8]
mov eax, dword ptr [$fibArr+eax*4]
mov ebx, 0
cmp eax, ebx
jne L1
mov eax, dword ptr [ebp+8]
sub eax, 1
push eax
mov dword ptr [ebp-4], eax
call fib
add esp, 4
mov ebx, dword ptr [ebp+8]
sub ebx, 2
push ebx
mov dword ptr [ebp-8], eax
mov dword ptr [ebp-12], ebx
call fib
add esp, 4
mov ebx, dword ptr [ebp-8]
add ebx, eax
mov ecx, dword ptr [ebp+8]
mov dword ptr [$fibArr+ecx*4], ebx
L1:
mov eax, dword ptr [ebp+8]
mov eax, dword ptr [$fibArr+eax*4]
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
mov ecx, dword ptr [ebp-8]
mov ecx, dword ptr [$globalArr+ecx*4]
mov edi, dword ptr [$globalArr+eax*4]
cmp ecx, edi
jle L6
mov ecx, dword ptr [ebp-8]
mov ecx, dword ptr [$globalArr+ecx*4]
mov dword ptr [ebp-16], ecx
mov ecx, dword ptr [ebp-8]
mov edi, dword ptr [$globalArr+eax*4]
mov dword ptr [$globalArr+ecx*4], edi
mov edi, dword ptr [ebp-16]
mov dword ptr [$globalArr+eax*4], edi
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

; isPrime
isPrime proc

; isPrime prologue
push ebp
mov ebp, esp
sub esp, 24
push esi
push edi
push ebx

; isPrime body
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L7
mov eax, 0
jmp @isPrime_epilogue
L7:
mov eax, dword ptr [ebp+8]
mov ebx, 2
cmp eax, ebx
jne L8
mov eax, 1
jmp @isPrime_epilogue
L8:
mov eax, dword ptr [ebp+8]
cdq
mov ebx, 2
idiv ebx
mov dword ptr [ebp-16], eax
mov dword ptr [ebp-4], eax
mov eax, dword ptr [ebp+8]
cdq
mov ebx, 2
idiv ebx
imul eax, 2
mov ebx, dword ptr [ebp+8]
cmp eax, ebx
jne L9
mov eax, 0
jmp @isPrime_epilogue
L9:
mov dword ptr [ebp-20], 3
L11:
mov eax, dword ptr [ebp-20]
mov ebx, dword ptr [ebp-16]
cmp eax, ebx
jge L10
mov dword ptr [ebp-20], eax
mov eax, dword ptr [ebp+8]
cdq
mov ecx, dword ptr [ebp-20]
idiv ecx
imul eax, ecx
mov edi, dword ptr [ebp+8]
cmp eax, edi
jne L12
mov eax, 0
jmp @isPrime_epilogue
L12:
mov eax, dword ptr [ebp-20]
add eax, 2
mov dword ptr [ebp-20], eax
jmp L11
L10:
mov eax, 1
jmp @isPrime_epilogue

; isPrime epilogue
@isPrime_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
isPrime endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 52
push esi
push edi
push ebx

; main body
lea eax, $str0
push eax
call printf
add esp, 4
lea eax, dword ptr [ebp-40]
push eax
lea eax, [$formatC]
push eax
call scanf
add esp, 8
mov eax, dword ptr [ebp-40]
add eax, 5
mov dword ptr [ebp-40], eax
mov dword ptr [ebp-36], eax
mov eax, 10
cdq
mov ebx, 3
idiv ebx
mov ebx, dword ptr [ebp-40]
neg eax
add eax, ebx
mov dword ptr [ebp-40], eax
mov ebx, dword ptr [ebp-40]
push ebx
lea ecx, $formatCNewLine
push ecx
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-40], ebx
call printf
add esp, 8
mov dword ptr [$fibArr+0*4], 1
mov dword ptr [$fibArr+1*4], 1
push 9
call fib
add esp, 4
mov dword ptr [ebp-44], 0
lea ebx, $str1
push ebx
mov dword ptr [ebp-8], eax
call printf
add esp, 4
L13:
mov eax, dword ptr [ebp-44]
mov eax, dword ptr [$fibArr+eax*4]
push eax
lea ebx, $formatDNewLine
push ebx
mov ebx, dword ptr [ebp-44]
mov dword ptr [$fibArr+ebx*4], eax
mov dword ptr [ebp-44], ebx
call printf
add esp, 8
mov eax, dword ptr [ebp-44]
add eax, 1
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-44]
mov ecx, 10
cmp ebx, ecx
jl L13
lea ecx, $str2
push ecx
mov dword ptr [ebp-12], eax
mov dword ptr [ebp-44], ebx
call printf
add esp, 4
lea eax, dword ptr [$n]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov eax, dword ptr [$n]
mov ebx, 10
cmp eax, ebx
jle L14
mov dword ptr [$n], 10
lea eax, $str3
push eax
call printf
add esp, 4
L14:
lea eax, $str4
push eax
call printf
add esp, 4
mov dword ptr [ebp-44], 0
L16:
mov eax, dword ptr [ebp-44]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L15
lea ecx, dword ptr [ebp-48]
push ecx
lea ecx, [$formatD]
push ecx
mov dword ptr [ebp-44], eax
mov dword ptr [$n], ebx
call scanf
add esp, 8
mov eax, dword ptr [ebp-44]
mov ebx, dword ptr [ebp-48]
mov dword ptr [$globalArr+eax*4], ebx
add eax, 1
mov dword ptr [ebp-44], eax
jmp L16
L15:
call sort
add esp, 0
mov dword ptr [ebp-44], 0
L18:
mov eax, dword ptr [ebp-44]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L17
mov ecx, dword ptr [$globalArr+eax*4]
push ecx
lea edi, $formatDNewLine
push edi
mov dword ptr [ebp-44], eax
mov dword ptr [$n], ebx
mov edi, dword ptr [ebp-44]
mov dword ptr [$globalArr+edi*4], ecx
call printf
add esp, 8
add edi, 1
mov dword ptr [ebp-44], edi
jmp L18
L17:
lea eax, $str5
push eax
call printf
add esp, 4
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
mov dword ptr [ebp-24], eax
mov dword ptr [ebp-20], ebx
mov dword ptr [ebp-16], ecx
call printf
add esp, 8
lea eax, $str6
push eax
call printf
add esp, 4
lea eax, dword ptr [$n]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, $str7
push eax
call printf
add esp, 4
mov dword ptr [ebp-44], 1
L19:
push dword ptr [ebp-44]
call isPrime
add esp, 4
mov ebx, 0
cmp eax, ebx
je L20
mov ebx, dword ptr [ebp-44]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-28], eax
mov dword ptr [ebp-44], ebx
call printf
add esp, 8
L20:
mov eax, dword ptr [ebp-44]
add eax, 1
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-44]
mov ecx, dword ptr [$n]
cmp ebx, ecx
jl L19

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
