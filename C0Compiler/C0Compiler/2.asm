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
$d dword 13 dup(0)


.code

; sort
@sort proc

; sort prologue
push ebp
mov ebp, esp
sub esp, 40
push esi
push edi
push ebx

; sort body
mov eax, dword ptr [ebp+8]
mov dword ptr [ebp-28], eax
mov ebx, dword ptr [ebp+12]
mov dword ptr [ebp-32], ebx
mov dword ptr [ebp+8], eax
add eax, ebx
mov dword ptr [ebp-4], eax
mov eax, dword ptr [ebp-4]
cdq
mov ecx, 2
idiv ecx
mov ecx, dword ptr [$d+eax*4]
mov dword ptr [ebp-36], ecx
L1:
mov eax, dword ptr [ebp-36]
mov ebx, dword ptr [ebp-32]
mov ebx, dword ptr [$d+ebx*4]
cmp eax, ebx
jge L2
L3:
mov eax, dword ptr [ebp-32]
sub eax, 1
mov dword ptr [ebp-32], eax
mov ebx, dword ptr [ebp-36]
mov ecx, dword ptr [ebp-32]
mov ecx, dword ptr [$d+ecx*4]
cmp ebx, ecx
jl L3
L2:
mov eax, dword ptr [ebp-36]
mov ebx, dword ptr [ebp-28]
mov ebx, dword ptr [$d+ebx*4]
cmp eax, ebx
jle L4
L5:
mov eax, dword ptr [ebp-28]
add eax, 1
mov dword ptr [ebp-28], eax
mov ebx, dword ptr [ebp-36]
mov ecx, dword ptr [ebp-28]
mov ecx, dword ptr [$d+ecx*4]
cmp ebx, ecx
jg L5
L4:
mov eax, dword ptr [ebp-28]
mov ebx, dword ptr [ebp-32]
cmp eax, ebx
jg L6
mov ecx, dword ptr [$d+eax*4]
mov dword ptr [ebp-24], ecx
mov ecx, dword ptr [$d+ebx*4]
mov dword ptr [$d+eax*4], ecx
mov ecx, dword ptr [ebp-24]
mov dword ptr [$d+ebx*4], ecx
mov dword ptr [ebp-28], eax
add eax, 1
mov dword ptr [ebp-28], eax
mov dword ptr [ebp-32], ebx
sub ebx, 1
mov dword ptr [ebp-32], ebx
L6:
mov eax, dword ptr [ebp-28]
mov ebx, dword ptr [ebp-32]
cmp eax, ebx
jle L1
mov ecx, dword ptr [ebp+12]
cmp eax, ecx
jge L7
push ecx
push eax
mov dword ptr [ebp-28], eax
mov dword ptr [ebp-32], ebx
mov dword ptr [ebp+12], ecx
call @sort
add esp, 8
L7:
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp-32]
cmp eax, ebx
jge L8
push ebx
push eax
mov dword ptr [ebp+8], eax
mov dword ptr [ebp-32], ebx
call @sort
add esp, 8
L8:

; sort epilogue
@sort_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@sort endp

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
mov eax, 356
cdq
mov ebx, 4
idiv ebx
add eax, 83
mov ebx, dword ptr [ebp-16]
mov ebx, 1012
sub eax, ebx
imul eax, 234
imul eax, -1
mov dword ptr [ebp-12], eax
neg eax
add eax, 0
mov dword ptr [$d+0*4], eax
mov ecx, dword ptr [ebp-24]
mov ecx, -3234
mov dword ptr [ebp-24], ecx
neg ecx
add ecx, 0
mov dword ptr [$d+1*4], ecx
mov edi, dword ptr [ebp-8]
mov edi, -57
mov dword ptr [$d+2*4], edi
push 2
push 0
mov dword ptr [ebp-20], eax
mov dword ptr [ebp-16], ebx
mov dword ptr [ebp-4], ecx
call @sort
add esp, 8
mov dword ptr [ebp-28], 0
L10:
mov eax, dword ptr [ebp-28]
mov ebx, 3
cmp eax, ebx
jge L9
mov ebx, dword ptr [$d+eax*4]
push ebx
lea ecx, $formatDNewLine
push ecx
mov dword ptr [ebp-28], eax
mov ecx, dword ptr [ebp-28]
mov dword ptr [$d+ecx*4], ebx
mov dword ptr [ebp-28], ecx
call printf
add esp, 8
mov eax, dword ptr [ebp-28]
add eax, 1
mov dword ptr [ebp-28], eax
jmp L10
L9:

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
