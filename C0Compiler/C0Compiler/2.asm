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


.code

; add
@add proc

; add prologue
push ebp
mov ebp, esp
sub esp, 8
push esi
push edi
push ebx

; add body
mov eax, dword ptr [ebp+8]
mov ebx, dword ptr [ebp+12]
add eax, ebx
jmp @add_epilogue

; add epilogue
@add_epilogue:
pop ebx
pop edi
pop esi
mov esp, ebp
pop ebp
ret
@add endp

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
mov dword ptr [ebp-20], 1
mov dword ptr [ebp-24], 2
mov eax, dword ptr [ebp-20]
mov ebx, dword ptr [ebp-24]
add eax, ebx
mov dword ptr [ebp-28], eax
mov ecx, dword ptr [ebp-20]
add ecx, ebx
mov edi, dword ptr [ebp-28]
add ecx, edi
push ebx
push dword ptr [ebp-20]
mov dword ptr [ebp-4], eax
mov dword ptr [ebp-24], ebx
mov dword ptr [ebp-8], ecx
call @add
add esp, 8
mov ebx, dword ptr [ebp-8]
add ebx, eax
mov ecx, dword ptr [ebp-20]
mov edx, dword ptr [ebp-24]
add ecx, edx
add ebx, ecx
mov dword ptr [ebp-32], ebx

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
