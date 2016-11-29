.386
.model flat, stdcall  
option casemap: none
includelib msvcrt.lib
printf proto c
scanf proto c


.data
$formatD  byte "%d", 0
$formatC  byte "%c", 0
$_x dword 0
$b dword 10
$c dword -10
$d dword 10
$fibs dword 10 dup(0)
$str dword 98
$str0 byte "factor ", 0
$str1 byte "input is ", 0
$str2 byte "input is ", 0
$str3 byte "test abs ", 0
$str4 byte "test factor ", 0
$str5 byte "program end", 0


.code

; abs
abs proc

; abs prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; abs body
mov eax, dword ptr [ebp+8]
mov ebx, 0
cmp eax, ebx
jle L1
jmp @abs_epilogue
jmp L2
L1:
mov eax, dword ptr [ebp+8]
sub eax, 0
jmp @abs_epilogue
L2:

; abs epilogue
@abs_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
abs endp

; factor
factor proc

; factor prologue
push ebp
mov ebp, esp
sub esp, 28
push esi
push edi
push ebx

; factor body
push dword ptr [ebp+8]
call abs
add esp, 4
mov dword ptr [ebp-20], eax
L3:
mov eax, dword ptr [ebp+8]
cdq
mov ebx, dword ptr [ebp-20]
idiv ebx
mov dword ptr [ebp-24], eax
mov ecx, dword ptr [ebp-24]
mov dword ptr [ebp-20], ebx
imul ebx, ecx
mov edi, dword ptr [ebp+8]
cmp edi, ebx
jne L4
lea edx, $str0
push edx
mov dword ptr [ebp-8], eax
call printf
add esp, 4
mov eax, dword ptr [ebp-20]
push eax
lea edx, $formatD
push edx
mov dword ptr [ebp-20], eax
mov dword ptr [ebp-24], ecx
call printf
add esp, 8
L4:
mov eax, dword ptr [ebp-20]
sub eax, 1
mov dword ptr [ebp-20], eax
mov ebx, dword ptr [ebp-20]
mov ecx, 0
cmp ebx, ecx
jg L3

; factor epilogue
@factor_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
factor endp

; oddif
oddif proc

; oddif prologue
push ebp
mov ebp, esp
sub esp, 16
push esi
push edi
push ebx

; oddif body
mov dword ptr [ebp-12], 2
push dword ptr [ebp+8]
call abs
add esp, 4
mov dword ptr [ebp+8], eax
mov ebx, dword ptr [ebp+8]
mov ecx, 0
cmp ebx, ecx
jne L5
mov eax, 0
jmp @oddif_epilogue
L5:
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L6
mov eax, 1
jmp @oddif_epilogue
L6:
L7:
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp-12]
sub eax, ebx
mov dword ptr [ebp+8], eax
mov ecx, dword ptr [ebp+8]
mov edi, 1
cmp ecx, edi
jg L7
mov edi, 0
cmp ecx, edi
je L8
mov eax, 1
jmp @oddif_epilogue
jmp L9
L8:
mov eax, 0
jmp @oddif_epilogue
L9:

; oddif epilogue
@oddif_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
oddif endp

; fibonacci
fibonacci proc

; fibonacci prologue
push ebp
mov ebp, esp
sub esp, 20
push esi
push edi
push ebx

; fibonacci body
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L10
mov eax, 1
jmp @fibonacci_epilogue
L10:
mov eax, dword ptr [ebp+8]
mov ebx, 2
cmp eax, ebx
jne L11
mov eax, 2
jmp @fibonacci_epilogue
jmp L12
L11:
mov eax, dword ptr [ebp+8]
sub eax, 1
push eax
mov dword ptr [ebp-12], eax
call fibonacci
add esp, 4
mov ebx, dword ptr [ebp+8]
sub ebx, 2
push ebx
mov dword ptr [ebp-16], eax
call fibonacci
add esp, 4
mov ecx, dword ptr [ebp-16]
add ecx, eax
mov eax, ecx
jmp @fibonacci_epilogue
L12:

; fibonacci epilogue
@fibonacci_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
fibonacci endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 108
push esi
push edi
push ebx

; main body
mov dword ptr [ebp-104], 43
mov dword ptr [ebp-56], 97
mov dword ptr [ebp-60], 0
L13:
mov eax, dword ptr [ebp-56]
mov ebx, 31
cmp eax, ebx
jg L14
mov ebx, dword ptr [ebp-60]
mov dword ptr [ebp-100+ebx*4], 48
mov dword ptr [ebp-60], ebx
add ebx, 1
mov dword ptr [ebp-60], ebx
jmp L15
L14:
mov eax, dword ptr [ebp-60]
mov ebx, dword ptr [ebp-56]
mov dword ptr [ebp-100+eax*4], ebx
mov dword ptr [ebp-60], eax
add eax, 1
mov dword ptr [ebp-60], eax
L15:
mov eax, dword ptr [ebp-56]
add eax, 1
mov dword ptr [ebp-56], eax
mov ebx, dword ptr [ebp-60]
mov ecx, 10
cmp ebx, ecx
jl L13
mov dword ptr [ebp-60], 9
L17:
mov eax, dword ptr [ebp-60]
mov ebx, 0
cmp eax, ebx
jl L16
mov ebx, dword ptr [ebp-100+eax*4]
push ebx
lea ecx, $formatC
push ecx
mov dword ptr [ebp-60], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-60]
sub eax, 1
mov dword ptr [ebp-60], eax
jmp L17
L16:
lea eax, dword ptr [ebp-60]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [$_x]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, $str1
push eax
call printf
add esp, 4
mov eax, dword ptr [ebp-60]
push eax
lea ebx, $formatD
push ebx
mov dword ptr [ebp-60], eax
call printf
add esp, 8
lea eax, $str2
push eax
call printf
add esp, 4
mov eax, dword ptr [$_x]
push eax
lea ebx, $formatC
push ebx
mov dword ptr [$_x], eax
call printf
add esp, 8
lea eax, $str3
push eax
call printf
add esp, 4
push dword ptr [ebp-60]
call abs
add esp, 4
push eax
lea ebx, $formatD
push ebx
mov dword ptr [ebp-16], eax
call printf
add esp, 8
lea eax, $str4
push eax
call printf
add esp, 4
push dword ptr [ebp-60]
call factor
add esp, 4
mov dword ptr [ebp-60], 0
L19:
mov eax, dword ptr [ebp-60]
mov ebx, 10
cmp eax, ebx
jge L18
mov dword ptr [ebp-60], eax
add eax, 1
push eax
mov dword ptr [ebp-20], eax
call fibonacci
add esp, 4
mov ebx, dword ptr [ebp-60]
mov dword ptr [$fibs+ebx*4], eax
add ebx, 1
mov dword ptr [ebp-60], ebx
jmp L19
L18:
mov dword ptr [ebp-60], 0
L20:
mov eax, dword ptr [ebp-60]
push dword ptr [$fibs+eax*4]
mov dword ptr [ebp-60], eax
call factor
add esp, 4
mov eax, dword ptr [ebp-60]
add eax, 1
mov dword ptr [ebp-60], eax
mov ebx, dword ptr [ebp-60]
mov ecx, 6
cmp ebx, ecx
jl L20
mov dword ptr [ebp-60], 0
L21:
mov eax, dword ptr [ebp-60]
push dword ptr [$fibs+eax*4]
mov dword ptr [ebp-60], eax
call oddif
add esp, 4
mov ebx, 1
cmp eax, ebx
jne L22
mov ebx, dword ptr [ebp-60]
push ebx
lea ecx, $formatD
push ecx
mov dword ptr [ebp-32], eax
call printf
add esp, 8
L22:
mov eax, dword ptr [ebp-60]
add eax, 1
mov dword ptr [ebp-60], eax
mov ebx, dword ptr [ebp-60]
mov ecx, 10
cmp ebx, ecx
jl L21
mov ecx, dword ptr [ebp-40]
mov ecx, 2
mov edx, dword ptr [$fibs+ecx*4]
mov edi, dword ptr [$fibs+edx*4]
imul edi, 10
imul edi, 98
add edi, -10
sub edi, 10
add edi, 1
mov dword ptr [ebp-36], eax
mov dword ptr [$fibs+ecx*4], edx
mov eax, 10
cdq
mov esi, 5
idiv esi
sub edi, eax
mov edx, dword ptr [ebp-52]
mov edx, -135
add edi, edx
mov dword ptr [ebp-60], edi
mov ebx, dword ptr [ebp-60]
push ebx
lea esi, $formatD
push esi
mov dword ptr [ebp-48], eax
mov dword ptr [ebp-40], ecx
mov dword ptr [ebp-52], edx
call printf
add esp, 8
lea eax, $str5
push eax
call printf
add esp, 4
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
