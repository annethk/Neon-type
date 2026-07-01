@echo off
echo Compilando o NeonType...
gcc -I include src/*.c -o neontype.exe -lraylib -lopengl32 -lgdi32 -lwinmm
if %errorlevel% equ 0 (
    echo Compilacao bem-sucedida!
    neontype.exe
) else (
    echo Erro na compilacao!
)
pause