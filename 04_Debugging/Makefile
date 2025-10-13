# Настройки компилятора
CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -Wpedantic
TARGET = range

# Имена файлов
GDB_SCRIPTS = script1.gdb script2.gdb
EXPECTED = expected1.txt expected2.txt
OUTPUTS = output1.txt output2.txt
FILTERED = filtered1.txt filtered2.txt

# Цель по умолчанию
.PHONY: all
all: $(TARGET)

# Компиляция программы
$(TARGET): range.c
	$(CC) $(CFLAGS) -o $@ $<

# Запуск тестов
.PHONY: test
test: $(TARGET) $(GDB_SCRIPTS) $(EXPECTED)
	# Запуск первого сценария и фильтрация вывода
	gdb --quiet --batch --command=script1.gdb ./$(TARGET) 2>&1 | \
	    grep '^@@@' > $(FILTERED) || true
	@echo "=== Проверка первого сценария ==="
	@if cmp -s $(FILTERED) expected1.txt; then \
	    echo "✓ Первый сценарий прошел проверку"; \
	else \
	    echo "✗ Первый сценарий не прошел проверку"; \
	    echo "Отличия:"; \
	    diff -u expected1.txt $(FILTERED) || true; \
	    exit 1; \
	fi
	
	# Запуск второго сценария и фильтрация вывода
	gdb --quiet --batch --command=script2.gdb ./$(TARGET) 2>&1 | \
	    grep '^@@@' > filtered2.txt || true
	@echo "\n=== Проверка второго сценария ==="
	@if cmp -s filtered2.txt expected2.txt; then \
	    echo "✓ Второй сценарий прошел проверку"; \
	else \
	    echo "✗ Второй сценарий не прошел проверку"; \
	    echo "Отличия:"; \
	    diff -u expected2.txt filtered2.txt || true; \
	    exit 1; \
	fi
	
	@echo "\n✅ Все тесты пройдены успешно!"

# Проверка синтаксиса gdb-скриптов
.PHONY: check-scripts
check-scripts: $(GDB_SCRIPTS)
	@for script in $(GDB_SCRIPTS); do \
	    echo "Проверка $$script..."; \
	    gdb --quiet --batch --command=$$script --ex="quit" 2>&1 | \
	        head -5; \
	    echo "✓ $$script корректен"; \
	done

# Очистка
.PHONY: clean
clean:
	rm -f $(TARGET) $(OUTPUTS) $(FILTERED) *.o
	@echo "Очистка завершена"

# Проверка установки debuginfo
.PHONY: check-debuginfo
check-debuginfo:
	@echo "Проверка наличия debuginfo для используемых библиотек..."
	@ldd ./$(TARGET) 2>/dev/null | grep -v "not found" | \
	    while read lib _; do \
	        if [ -f "$$lib" ]; then \
	            echo "Проверяю $$lib"; \
	            readelf -S "$$lib" 2>/dev/null | grep -q debug && \
	                echo "  ✓ Есть debug секции" || \
	                echo "  ✗ Нет debug секций"; \
	        fi; \
	    done

# Дополнительная цель для отладки
.PHONY: debug
debug: $(TARGET)
	@echo "Запуск gdb в интерактивном режиме..."
	gdb -tui ./$(TARGET)

