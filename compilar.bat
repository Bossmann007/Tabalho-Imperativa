@echo off
chcp 65001 > nul
echo =====================================================
echo  Compilando P1 e P2 (PUCPR - Programacao Imperativa)
echo =====================================================
echo.

echo [1/2] Compilando P1.exe...
gcc -Wall -Wextra -std=c99 -pedantic-errors -Wmissing-prototypes P1.c alimento.c arquivo.c interface.c lista.c arvore.c -o P1.exe
if %ERRORLEVEL% neq 0 (
    echo ERRO na compilacao de P1!
    pause
    exit /b %ERRORLEVEL%
)
echo P1.exe compilado com sucesso!

echo.
echo [2/2] Compilando P2.exe...
gcc -Wall -Wextra -std=c99 -pedantic-errors -Wmissing-prototypes P2.c alimento.c arquivo.c arvore.c lista.c interface.c -o P2.exe
if %ERRORLEVEL% neq 0 (
    echo ERRO na compilacao de P2!
    pause
    exit /b %ERRORLEVEL%
)
echo P2.exe compilado com sucesso!

echo.
echo =====================================================
echo  Compilacao concluida com ZERO erros e avisos!
echo =====================================================
echo.
echo Escolha o programa para executar:
echo [1] Executar P1.exe (Gerar dados.bin a partir do CSV)
echo [2] Executar P2.exe (Menu interativo de Listas e Arvores)
echo [3] Sair
set /p escolha="Opcao (1-3): "

if "%escolha%"=="1" (
    cls
    P1.exe
)
if "%escolha%"=="2" (
    cls
    P2.exe
)
pause
