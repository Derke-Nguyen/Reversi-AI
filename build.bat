@echo off
call gcc src/main.c src/movelist.c src/game.c src/aiopponent.c -o build/ReversiAI.exe -O1 -Wall -std=c99 -Wno-missing-braces -I src/include/ -L src/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm