# Компилятор
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fPIC
AR = ar
ARFLAGS = rcs

# Имена библиотек
STATIC_LIB = liboutput_static.a
DYNAMIC_LIB = liboutput.so

# Имена исполняемых файлов
PROG = prog
PROG_STATIC = prog-a
PROG_DYNAMIC = prog-so

# Исходные файлы
SRCS = const.c fun.c prog.c
OBJS = $(SRCS:.c=.o)
LIB_OBJS = fun.o const.o

# Цели по умолчанию
.PHONY: all
all: $(PROG) $(PROG_STATIC) $(PROG_DYNAMIC)

# Сборка из отдельных .o файлов
$(PROG): prog.o fun.o const.o
	$(CC) -o $@ $^

# Сборка со статической библиотекой
$(PROG_STATIC): prog.o $(STATIC_LIB)
	$(CC) -o $@ prog.o -L. -loutput_static

# Сборка с динамической библиотекой
$(PROG_DYNAMIC): prog.o $(DYNAMIC_LIB)
	$(CC) -o $@ prog.o -L. -loutput -Wl,-rpath,.

# Статическая библиотека
$(STATIC_LIB): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Динамическая библиотека
$(DYNAMIC_LIB): CFLAGS += -fPIC
$(DYNAMIC_LIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $^

# Правило для компиляции .c файлов в .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Заголовочные файлы как зависимости
const.o: const.c const.h
fun.o: fun.c fun.h
prog.o: prog.c const.h fun.h

# Тестирование
.PHONY: test
test: all
	# Тест без параметров
	./$(PROG) > prog.out 2>&1
	./$(PROG_STATIC) > prog-a.out 2>&1
	./$(PROG_DYNAMIC) > prog-so.out 2>&1
	cmp prog.out prog-a.out
	cmp prog.out prog-so.out
	@echo "✓ Тест без параметров пройден"
	
	# Тест с одним параметром
	./$(PROG) param1 > prog.param1.out 2>&1
	./$(PROG_STATIC) param1 > prog-a.param1.out 2>&1
	./$(PROG_DYNAMIC) param1 > prog-so.param1.out 2>&1
	cmp prog.param1.out prog-a.param1.out
	cmp prog.param1.out prog-so.param1.out
	@echo "✓ Тест с одним параметром пройден"
	
	# Тест с тремя параметрами
	./$(PROG) param1 param2 param3 > prog.param3.out 2>&1
	./$(PROG_STATIC) param1 param2 param3 > prog-a.param3.out 2>&1
	./$(PROG_DYNAMIC) param1 param2 param3 > prog-so.param3.out 2>&1
	cmp prog.param3.out prog-a.param3.out
	cmp prog.param3.out prog-so.param3.out
	@echo "✓ Тест с тремя параметрами пройден"
	@echo "✅ Все тесты пройдены успешно!"

# Очистка всех сгенерированных файлов
.PHONY: clean
clean:
	rm -f $(OBJS) $(STATIC_LIB) $(DYNAMIC_LIB) \
	      $(PROG) $(PROG_STATIC) $(PROG_DYNAMIC) \
	      *.out

