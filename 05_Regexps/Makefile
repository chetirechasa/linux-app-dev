# Настройки компилятора
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g
TARGET = esub

# Цель по умолчанию
.PHONY: all
all: $(TARGET)

# Компиляция программы
$(TARGET): esub.c
	$(CC) $(CFLAGS) -o $@ $<

# Тестирование
.PHONY: test
test: $(TARGET)
	@echo "=== Запуск тестов ==="
	
	# Тест 1: Простая замена
	@echo "Тест 1: Простая замена"
	@./$(TARGET) 'test' 'TEST' 'This is a test string' > out1.txt
	@echo 'This is a TEST string' > expected1.txt
	@if diff -u out1.txt expected1.txt; then \
		echo "✓ Тест 1 пройден"; \
	else \
		echo "✗ Тест 1 не пройден"; \
	fi
	
	# Тест 2: Замена с карманом
	@echo "\nТест 2: Замена с карманом"
	@./$(TARGET) '([0-9]+)' 'ЧИСЛО: \1' 'Тест 123 тест' > out2.txt
	@echo 'Тест ЧИСЛО: 123 тест' > expected2.txt
	@if diff -u out2.txt expected2.txt; then \
		echo "✓ Тест 2 пройден"; \
	else \
		echo "✗ Тест 2 не пройден"; \
	fi
	
	# Тест 3: Несколько карманов
	@echo "\nТест 3: Несколько карманов"
	@./$(TARGET) '([0-9]+)\.([0-9]+)' '\2.\1' '123.456' > out3.txt
	@echo '456.123' > expected3.txt
	@if diff -u out3.txt expected3.txt; then \
		echo "✓ Тест 3 пройден"; \
	else \
		echo "✗ Тест 3 не пройден"; \
	fi
	
	# Тест 4: Экранирование обратного слеша
	@echo "\nТест 4: Экранирование обратного слеша"
	@./$(TARGET) 'test' 'тест: \\' 'This is test' > out4.txt
	@echo 'This is тест: \' > expected4.txt
	@if diff -u out4.txt expected4.txt; then \
		echo "✓ Тест 4 пройден"; \
	else \
		echo "✗ Тест 4 не пройден"; \
	fi
	
	# Тест 5: Сравнение с sed
	@echo "\nТест 5: Сравнение с sed"
	@./$(TARGET) '([A-Za-z]+) ([A-Za-z]+)' '\2, \1' 'John Doe' > out5.txt
	@echo 'John Doe' | sed -E 's/([A-Za-z]+) ([A-Za-z]+)/\2, \1/' > expected5.txt
	@if diff -u out5.txt expected5.txt; then \
		echo "✓ Тест 5 пройден (совпадает с sed)"; \
	else \
		echo "✗ Тест 5 не пройден"; \
	fi
	
	# Тест 6: Ошибка - несуществующий карман
	@echo "\nТест 6: Проверка ошибки несуществующего кармана"
	@./$(TARGET) 'test' '\2' 'test string' 2>&1 | grep -q "несуществующий карман" && \
		echo "✓ Тест 6 пройден (ошибка обнаружена)" || \
		echo "✗ Тест 6 не пройден"
	
	# Тест 7: Ошибка - некорректное регулярное выражение
	@echo "\nТест 7: Проверка ошибки регулярного выражения"
	@./$(TARGET) '([0-9]+' 'replacement' 'test' 2>&1 | grep -q "Ошибка компиляции" && \
		echo "✓ Тест 7 пройден (ошибка обнаружена)" || \
		echo "✗ Тест 7 не пройден"
	
	@echo "\n=== Результаты тестирования ==="
	@echo "Для просмотра подробных отличий смотрите файлы out*.txt и expected*.txt"
	@echo "Для очистки результатов выполните: make clean"

# Тестирование с расширенным выводом
.PHONY: test-verbose
test-verbose: $(TARGET)
	@echo "=== Подробное тестирование ==="
	@echo "1. Тест простой замены:"
	@echo "   Вход: ./esub 'test' 'TEST' 'This is a test string'"
	@./$(TARGET) 'test' 'TEST' 'This is a test string'
	@echo "   Ожидалось: This is a TEST string"
	@echo
	
	@echo "2. Тест с карманом:"
	@echo "   Вход: ./esub '([0-9]+)' 'ЧИСЛО: \\\\1' 'Тест 123 тест'"
	@./$(TARGET) '([0-9]+)' 'ЧИСЛО: \1' 'Тест 123 тест'
	@echo "   Ожидалось: Тест ЧИСЛО: 123 тест"
	@echo
	
	@echo "3. Тест с несколькими карманами:"
	@echo "   Вход: ./esub '([0-9]+)\\.([0-9]+)' '\\\\2.\\\\1' '123.456'"
	@./$(TARGET) '([0-9]+)\.([0-9]+)' '\2.\1' '123.456'
	@echo "   Ожидалось: 456.123"
	@echo
	
	@echo "4. Тест экранирования:"
	@echo "   Вход: ./esub 'test' 'тест: \\\\\\\\' 'This is test'"
	@./$(TARGET) 'test' 'тест: \\' 'This is test'
	@echo "   Ожидалось: This is тест: \\"
	@echo
	
	@echo "5. Тест с sed для сравнения:"
	@echo "   esub:"
	@./$(TARGET) '([A-Za-z]+) ([A-Za-z]+)' '\2, \1' 'John Doe'
	@echo "   sed:"
	@echo 'John Doe' | sed -E 's/([A-Za-z]+) ([A-Za-z]+)/\2, \1/'

# Проверка стиля кодирования
.PHONY: check-style
check-style:
	@echo "Проверка стиля кодирования..."
	@echo "Замечания по стилю будут выведены ниже:"
	@if grep -n "^\t*printf" esub.c | grep -v '\\[nt]' | head -5; then \
		echo "Найдены printf без форматирования"; \
	fi
	@if grep -n "^\t*[0-9]" esub.c | head -5; then \
		echo "Найдены магические числа"; \
	fi

# Очистка
.PHONY: clean
clean:
	rm -f $(TARGET) out*.txt expected*.txt
	@echo "Очистка завершена"

# Справка
.PHONY: help
help:
	@echo "Доступные цели:"
	@echo "  all         - сборка программы (по умолчанию)"
	@echo "  test        - запуск автоматических тестов"
	@echo "  test-verbose - запуск тестов с подробным выводом"
	@echo "  check-style - проверка стиля кодирования"
	@echo "  clean       - удаление сгенерированных файлов"
	@echo "  help        - вывод этой справки"

