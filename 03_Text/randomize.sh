#!/bin/sh

# Функция для гарантированного удаления временных файлов
cleanup() {
    trap - EXIT INT TERM HUP  # Снимаем обработчики
    [ -n "$TMPFILE" ] && rm -f "$TMPFILE" 2>/dev/null
    exit
}

# Устанавливаем обработчик сигналов для гарантированной очистки
trap cleanup EXIT INT TERM HUP

# Обработка аргумента времени задержки
DELAY=0.05
if [ $# -ge 1 ]; then
    DELAY="$1"
fi

# Создаем временный файл для хранения данных
TMPFILE=$(mktemp) || exit 1

# Читаем весь ввод и сохраняем во временный файл
cat > "$TMPFILE"

# Получаем размеры терминала
TERM_LINES=$(stty size < /dev/tty | cut -d' ' -f1)
TERM_COLS=$(stty size < /dev/tty | cut -d' ' -f2)

# Перемещаем курсор в левый верхний угол
/bin/echo -n "\033[H"

# Подготавливаем символы для случайного вывода
# Собираем все символы из файла (игнорируя переводы строк)
CHAR_LIST=$(tr -d '\n' < "$TMPFILE")

# Преобразуем строку символов в список позиций
INDEX=1
while [ $INDEX -le ${#CHAR_LIST} ]; do
    CHAR=$(printf '%s' "$CHAR_LIST" | cut -c "$INDEX")
    printf "%d:%s\n" "$INDEX" "$CHAR"
    INDEX=$((INDEX + 1))
done > "${TMPFILE}_positions"

# Перемешиваем позиции
shuf "${TMPFILE}_positions" > "${TMPFILE}_shuffled"

# Выводим символы в случайном порядке
while IFS=: read -r POS CHAR; do
    # Пропускаем пустые строки
    [ -z "$POS" ] && continue
    
    # Перемещаем курсор в нужную позицию
    # Вычисляем строку и колонку на основе исходной позиции
    ROW=1
    COL=$POS
    LINES_IN_FILE=$(wc -l < "$TMPFILE")
    COLS_IN_FILE=$(awk 'length > max {max = length} END {print max}' "$TMPFILE")
    
    # Простой расчет позиции (можно усовершенствовать)
    if [ "$COLS_IN_FILE" -gt 0 ]; then
        ROW=$(((POS - 1) / COLS_IN_FILE + 1))
        COL=$(((POS - 1) % COLS_IN_FILE + 1))
    fi
    
    # Убедимся, что позиция в пределах экрана
    if [ "$ROW" -le "$TERM_LINES" ] && [ "$COL" -le "$TERM_COLS" ]; then
        /bin/echo -n "\033[${ROW};${COL}H"
        /bin/echo -n "$CHAR"
        sleep "$DELAY"
    fi
done < "${TMPFILE}_shuffled"

# Перемещаем курсор вниз после завершения
/bin/echo -n "\033[$((LINES_IN_FILE + 1));1H"

