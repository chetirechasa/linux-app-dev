# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Основные цели
all: maze maze1 maze2 maze3

# Базовая версия
maze: maze.c
	$(CC) $(CFLAGS) -o maze maze.c

# Версия 1 (размер из командной строки)
maze1: maze_v1.c
	$(CC) $(CFLAGS) -o maze1 maze_v1.c

# Версия 2 (символы и размер из командной строки)
maze2: maze_v2.c
	$(CC) $(CFLAGS) -o maze2 maze_v2.c

# Версия 3 (seed, символы и размер из командной строки)
maze3: maze_v3.c
	$(CC) $(CFLAGS) -o maze3 maze_v3.c

# Создание патчей из готовых версий
patches: patch1.patch patch2.patch patch3.patch

patch1.patch: maze.c maze_v1.c
	diff -u maze.c maze_v1.c > patch1.patch

patch2.patch: maze_v1.c maze_v2.c
	diff -u maze_v1.c maze_v2.c > patch2.patch

patch3.patch: maze_v2.c maze_v3.c
	diff -u maze_v2.c maze_v3.c > patch3.patch

# Применение патчей для демонстрации
apply-patches:
	cp maze.c maze_patched1.c
	patch maze_patched1.c < patch1.patch
	cp maze_patched1.c maze_patched2.c
	patch maze_patched2.c < patch2.patch
	cp maze_patched2.c maze_patched3.c
	patch maze_patched3.c < patch3.patch

# Запуск всех версий
run: all
	@echo "=== Базовая версия (maze) ==="
	./maze
	@echo -e "\n=== Версия 1 (maze1) - размер 8 ==="
	./maze1 8
	@echo -e "\n=== Версия 2 (maze2) - символы +*, размер 10 ==="
	./maze2 "+*" 10
	@echo -e "\n=== Версия 3 (maze3) - seed 42, символы @_, размер 12 ==="
	./maze3 42 "@_" 12

# Очистка
clean:
	rm -f maze maze1 maze2 maze3 maze_patched*.c *.rej *.orig

.PHONY: all run clean patches apply-patches

