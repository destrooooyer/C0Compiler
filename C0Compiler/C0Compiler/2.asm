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
@fib proc

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
call @fib
add esp, 4
mov ebx, dword ptr [ebp+8]
sub ebx, 2
push ebx
mov dword ptr [ebp-8], eax
mov dword ptr [ebp-12], ebx
call @fib
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
@fib endp

; sort
@sort proc

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
@sort endp

; isPrime
@isPrime proc

; isPrime prologue
push ebp
mov ebp, esp
sub esp, 32
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
mov dword ptr [ebp-8], eax
mov ebx, dword ptr [ebp-8]
imul ebx, 2
mov dword ptr [ebp-24], eax
mov ecx, dword ptr [ebp+8]
cmp ebx, ecx
jne L9
mov eax, 0
jmp @isPrime_epilogue
L9:
mov dword ptr [ebp-28], 3
L11:
mov eax, dword ptr [ebp-28]
mov ebx, dword ptr [ebp-24]
cmp eax, ebx
jge L10
mov dword ptr [ebp-28], eax
mov eax, dword ptr [ebp+8]
cdq
mov ecx, dword ptr [ebp-28]
idiv ecx
mov dword ptr [ebp-4], eax
imul eax, ecx
mov edi, dword ptr [ebp+8]
cmp eax, edi
jne L12
mov eax, 0
jmp @isPrime_epilogue
L12:
mov eax, dword ptr [ebp-28]
add eax, 2
mov dword ptr [ebp-28], eax
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
@isPrime endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 84
push esi
push edi
push ebx

; main body
lea eax, $str0
push eax
call printf
add esp, 4
lea eax, dword ptr [ebp-72]
push eax
lea eax, [$formatC]
push eax
call scanf
add esp, 8
mov eax, 10
cdq
mov ebx, 3
idiv ebx
mov ebx, dword ptr [ebp-72]
add ebx, 5
mov dword ptr [ebp-20], ebx
mov ecx, dword ptr [ebp-20]
sub ecx, eax
mov dword ptr [ebp-72], ecx
mov edi, dword ptr [ebp-72]
push edi
lea edx, $formatCNewLine
push edx
mov dword ptr [ebp-28], eax
mov dword ptr [ebp-68], ebx
mov dword ptr [ebp-36], ecx
call printf
add esp, 8
mov dword ptr [$fibArr+0*4], 1
mov dword ptr [$fibArr+1*4], 1
push 9
call @fib
add esp, 4
mov dword ptr [ebp-76], 0
lea ebx, $str1
push ebx
mov dword ptr [ebp-40], eax
call printf
add esp, 4
L13:
mov eax, dword ptr [ebp-76]
mov eax, dword ptr [$fibArr+eax*4]
push eax
lea ebx, $formatDNewLine
push ebx
mov ebx, dword ptr [ebp-76]
mov dword ptr [$fibArr+ebx*4], eax
mov dword ptr [ebp-76], ebx
call printf
add esp, 8
mov eax, dword ptr [ebp-76]
add eax, 1
mov dword ptr [ebp-76], eax
mov ebx, dword ptr [ebp-76]
mov ecx, 10
cmp ebx, ecx
jl L13
lea ecx, $str2
push ecx
mov dword ptr [ebp-44], eax
mov dword ptr [ebp-76], ebx
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
mov dword ptr [ebp-76], 0
L16:
mov eax, dword ptr [ebp-76]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L15
lea ecx, dword ptr [ebp-80]
push ecx
lea ecx, [$formatD]
push ecx
mov dword ptr [ebp-76], eax
mov dword ptr [$n], ebx
call scanf
add esp, 8
mov eax, dword ptr [ebp-76]
mov ebx, dword ptr [ebp-80]
mov dword ptr [$globalArr+eax*4], ebx
add eax, 1
mov dword ptr [ebp-76], eax
jmp L16
L15:
call @sort
add esp, 0
mov dword ptr [ebp-76], 0
L18:
mov eax, dword ptr [ebp-76]
mov ebx, dword ptr [$n]
cmp eax, ebx
jge L17
mov ecx, dword ptr [$globalArr+eax*4]
push ecx
lea edi, $formatDNewLine
push edi
mov dword ptr [ebp-76], eax
mov dword ptr [$n], ebx
mov edi, dword ptr [ebp-76]
mov dword ptr [$globalArr+edi*4], ecx
call printf
add esp, 8
add edi, 1
mov dword ptr [ebp-76], edi
jmp L18
L17:
lea eax, $str5
push eax
call printf
add esp, 4
mov eax, 4
cdq
mov ebx, 3
idiv ebx
mov ebx, dword ptr [ebp-32]
mov ebx, -40
mov dword ptr [ebp-32], ebx
imul ebx, eax
mov dword ptr [ebp-4], ebx
add ebx, 432
mov dword ptr [ebp-8], ebx
sub ebx, 5
mov dword ptr [ebp-12], ebx
add ebx, 3
mov dword ptr [ebp-56], eax
mov eax, 20
cdq
mov ecx, 10
idiv ecx
mov dword ptr [ebp-16], eax
add eax, 50
mov dword ptr [ebp-24], eax
sub eax, ebx
mov dword ptr [$n], eax
mov ecx, dword ptr [$n]
push ecx
lea edi, $formatDNewLine
push edi
mov dword ptr [ebp-48], eax
mov dword ptr [ebp-52], ebx
mov dword ptr [$n], ecx
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
mov dword ptr [ebp-76], 1
L19:
push dword ptr [ebp-76]
call @isPrime
add esp, 4
mov ebx, 0
cmp eax, ebx
je L20
mov ebx, dword ptr [ebp-76]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-60], eax
mov dword ptr [ebp-76], ebx
call printf
add esp, 8
L20:
mov eax, dword ptr [ebp-76]
add eax, 1
mov dword ptr [ebp-76], eax
mov ebx, dword ptr [ebp-76]
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
