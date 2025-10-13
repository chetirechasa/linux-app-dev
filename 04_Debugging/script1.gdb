# Запуск программы с параметрами 1 12
set pagination off
set verbose off

# Задаем точку останова в цикле
break range.c:54 if (i - start) % 5 == 0
commands
    # Используем маркер для фильтрации
    echo @@@
    print start
    echo @@@
    print stop
    echo @@@
    print step
    echo @@@
    print i
    echo @@@\n
    continue
end

# Запускаем программу, перенаправляя ее вывод
run 1 12 > /dev/null

quit

