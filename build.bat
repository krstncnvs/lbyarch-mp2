nasm -f win64 imgCvtGrayFloatToInt.asm -o imgCvtGrayFloatToInt.obj
gcc -m64 main.c imgCvtGrayFloatToInt.obj -o program.exe
program.exe