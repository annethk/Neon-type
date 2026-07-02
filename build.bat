@echo off
:: Adiciona o caminho do compilador ao PATH temporariamente
set PATH=C:\msys64\ucrt64\bin;%PATH%

echo Compilando o NeonType...
gcc -Iinclude src/*.c -o neontype.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

if %errorlevel% equ 0 (
    echo Compilacao bem-sucedida!
    neontype.exe
) else (
    echo Erro na compilacao!
    echo Verifique se o MSYS2 esta instalado em C:\msys64
)
pause