gcc -c -ansi menuTrees.c -I. -Ilib/ -o menuTrees.o
gcc -c -ansi menuTreesMain.c -I. -Ilib/ -o menuTreesMain.o
gcc -c -ansi menuStruct.c -I. -Ilib/ -o menuStruct.o
gcc menuTrees.o menuTreesMain.o menuStruct.o -Llib/ -lm -o menuTrees