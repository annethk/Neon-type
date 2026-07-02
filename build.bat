@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%

echo Preparando o icone...
windres recursos.rc -O coff -o recursos.res

echo Compilando o NeonType com icone...
gcc -Iinclude src/*.c recursos.res -o neontype.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

if %errorlevel% equ 0 (
    echo Compilacao bem-sucedida!
    del recursos.res
    neontype.exe
) else (
    echo Erro na compilacao!
    pause
)