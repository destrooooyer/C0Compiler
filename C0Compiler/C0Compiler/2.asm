.386
.model flat, stdcall  
option casemap: none
includelib msvcrt.lib
printf proto c
scanf proto c


.data
$formatD  byte "%d", 0
$formatC  byte "%c", 0
$a dword 0
$a1 dword 0
$a2 dword 0
$c1 dword -5
$c2 dword 0
$c3 dword 6
$c4 dword 97
$c5 dword 45
$c6 dword 42
$d dword 0
$d1 dword 0
$d2 dword 0
$f dword 5 dup(0)
$g dword 6 dup(0)
$str0 byte "i=0", 0
$str1 byte "i!=0", 0
$str2 byte "j!=0", 0


.code

; f1
f1 proc

; f1 prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; f1 body
mov eax, dword ptr [ebp+8]
mov ebx, 1
add eax, ebx
jmp @f1_epilogue

; f1 epilogue
@f1_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
f1 endp

; f2
f2 proc

; f2 prologue
push ebp
mov ebp, esp
sub esp, 28
push esi
push edi
push ebx

; f2 body
mov dword ptr [ebp-16], 0
mov eax, dword ptr [ebp-4]
mov eax, -12
mov dword ptr [ebp-20], eax
mov dword ptr [ebp-24], 320
mov dword ptr [ebp+8], 45
mov dword ptr [ebp+12], 50
mov dword ptr [ebp-8], 47
mov dword ptr [ebp-12], 95
mov eax, 97
jmp @f2_epilogue

; f2 epilogue
@f2_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
f2 endp

; recursion
recursion proc

; recursion prologue
push ebp
mov ebp, esp
sub esp, 12
push esi
push edi
push ebx

; recursion body
mov eax, dword ptr [ebp+8]
mov ebx, 1
cmp eax, ebx
jne L1
mov eax, 1
jmp @recursion_epilogue
jmp L2
L1:
mov eax, dword ptr [ebp+8]
mov ebx, 1
sub eax, ebx
push eax
mov dword ptr [ebp-4], eax
call recursion
add esp, 4
mov ebx, dword ptr [ebp+8]
add eax, ebx
jmp @recursion_epilogue
L2:

; recursion epilogue
@recursion_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
recursion endp

; f3
f3 proc

; f3 prologue
push ebp
mov ebp, esp
sub esp, 76
push esi
push edi
push ebx

; f3 body
mov dword ptr [ebp-72+0*4], 0
mov eax, dword ptr [ebp-72+0*4]
mov ebx, 5
add eax, ebx
mov dword ptr [ebp-72+1*4], eax
mov dword ptr [ebp-48+0*4], 49
mov dword ptr [ebp-48+1*4], 50
mov dword ptr [ebp-52], 1
mov dword ptr [ebp-56], 2
mov dword ptr [ebp-60], -3
mov ebx, dword ptr [ebp-48+1*4]
mov dword ptr [ebp-40], ebx
mov ecx, dword ptr [ebp-52]
mov edi, dword ptr [ebp-56]
add ecx, edi
mov edx, dword ptr [ebp-60]
sub edx, edi
imul ecx, edx
mov esi, dword ptr [ebp-72+0*4]
mov dword ptr [ebp-20], eax
mov eax, dword ptr [ebp-72+1*4]
sub esi, eax
sub ecx, esi
mov dword ptr [ebp-72+1*4], eax
mov eax, dword ptr [ebp-48+0*4]
add eax, edi
mov dword ptr [ebp-48+1*4], ebx
mov ebx, dword ptr [ebp-40]
add eax, ebx
mov dword ptr [ebp-36], eax
mov dword ptr [ebp-28], edx
mov eax, ecx
cdq
mov dword ptr [ebp-40], ebx
mov ebx, dword ptr [ebp-36]
idiv ebx
mov dword ptr [ebp-24], eax
mov eax, 0
sub eax, eax
mov ecx, dword ptr [ebp-52]
imul eax, ecx
mov dword ptr [ebp-4], eax
mov eax, 0
sub eax, eax
mov dword ptr [ebp-8], eax
mov eax, dword ptr [ebp-40]
cdq
mov dword ptr [ebp-36], ebx
mov ebx, -3
idiv ebx
mov ebx, dword ptr [ebp-8]
sub ebx, eax
mov dword ptr [ebp-64], ebx
mov edx, dword ptr [ebp-64]
push edx
mov dword ptr [ebp-12], eax
lea eax, $formatD
push eax
mov dword ptr [ebp-52], ecx
mov dword ptr [ebp-64], edx
call printf
add esp, 8
push dword ptr [ebp-40]
push dword ptr [ebp-52]
call f1
add esp, 8
mov dword ptr [ebp-60], eax

; f3 epilogue
@f3_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
f3 endp

; main
main proc

; main prologue
push ebp
mov ebp, esp
sub esp, 76
push esi
push edi
push ebx

; main body
lea eax, dword ptr [ebp-44]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
lea eax, dword ptr [ebp-48]
push eax
lea eax, [$formatD]
push eax
call scanf
add esp, 8
mov dword ptr [ebp-36], 97
mov eax, dword ptr [ebp-44]
mov ebx, 0
cmp eax, ebx
jne L3
lea ebx, $str0
push ebx
mov dword ptr [ebp-44], eax
call printf
add esp, 4
jmp L4
L3:
lea eax, $str1
push eax
call printf
add esp, 4
L4:
mov eax, dword ptr [ebp-48]
mov ebx, 0
cmp eax, ebx
je L5
lea ebx, $str2
push ebx
mov dword ptr [ebp-48], eax
call printf
add esp, 4
L5:
mov dword ptr [ebp-44], 2
mov dword ptr [ebp-48], 0
L6:
mov eax, dword ptr [ebp-44]
mov ebx, 1
sub eax, ebx
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-48]
mov ecx, dword ptr [ebp-44]
mov dword ptr [ebp-68+ebx*4], ecx
mov dword ptr [ebp-48], ebx
mov ebx, 1
add ebx, ebx
mov dword ptr [ebp-48], ebx
mov edi, 0
cmp ecx, edi
jg L6
lea edi, dword ptr [ebp-48]
push edi
lea edi, [$formatD]
push edi
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-44], ecx
call scanf
add esp, 8
mov dword ptr [ebp-44], 0
L8:
mov eax, dword ptr [ebp-44]
mov ebx, dword ptr [ebp-48]
cmp eax, ebx
jge L7
mov dword ptr [ebp-44], eax
add eax, ebx
push eax
lea ecx, $formatD
push ecx
mov dword ptr [ebp-12], eax
call printf
add esp, 8
mov eax, dword ptr [ebp-44]
add eax, 1
mov dword ptr [ebp-44], eax
jmp L8
L7:
mov eax, dword ptr [ebp-48]
mov ebx, dword ptr [ebp-44]
add eax, ebx
push 97
push eax
mov dword ptr [ebp-16], eax
call f1
add esp, 8
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-44]
push ebx
lea ecx, $formatD
push ecx
mov dword ptr [ebp-20], eax
call printf
add esp, 8
push 99
push ebx
call f2
add esp, 8
mov dword ptr [ebp-36], eax
mov ecx, dword ptr [ebp-36]
push ecx
lea edi, $formatC
push edi
mov dword ptr [ebp-24], eax
mov dword ptr [ebp-36], ecx
call printf
add esp, 8
call f3
add esp, 0
push 1
call recursion
add esp, 4
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-44]
push ebx
lea ecx, $formatD
push ecx
mov dword ptr [ebp-28], eax
call printf
add esp, 8
push 3
call recursion
add esp, 4
mov dword ptr [ebp-44], eax
mov ebx, dword ptr [ebp-44]
push ebx
lea ecx, $formatD
push ecx
mov dword ptr [ebp-32], eax
call printf
add esp, 8
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
