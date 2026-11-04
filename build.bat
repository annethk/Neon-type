@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%

echo Preparando o icone...
windres recursos.rc -O coff -o recursos.res

echo Compilando o NeonType com icone...
gcc -std=c99 -Iinclude src/*.c recursos.res -o neontype.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32 -mwindows

if %errorlevel% equ 0 (
    echo Compilacao bem-sucedida!
    
    REM Garantir que as DLLs essenciais estejam na raiz
    copy /Y C:\msys64\ucrt64\bin\libraylib.dll . >nul
    copy /Y C:\msys64\ucrt64\bin\glfw3.dll . >nul
    
    REM Limpar arquivos temporarios de compilacao
    del recursos.res
    
    echo.
    echo ===================================
    echo Jogo compilado e atualizado na raiz!
    echo ===================================
) else (
    echo Erro na compilacao!
    pause
)