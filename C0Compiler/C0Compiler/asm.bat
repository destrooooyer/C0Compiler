set include=c:\masm32\include
set lib=c:\masm32\lib
set path=c:\masm32\bin;%path%
ml -c -coff 2.asm
link -subsystem:console 2.obj